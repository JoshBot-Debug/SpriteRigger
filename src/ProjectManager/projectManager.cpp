#include "projectManager.h"
#include "Serializer/serializer.h"

ProjectManager::ProjectManager(std::string projectFileName, std::string recentFileName, int maxRecentProjects) : projectFileName(projectFileName), recentFileName(recentFileName), maxRecentProjects(maxRecentProjects)
{
  // Load recent projects into recent
  std::filesystem::path recentFilePath = std::filesystem::current_path() / this->recentFileName;

  Serializer::in(recentFilePath, [this](auto &stream)
                 {
                   for (size_t i = 0; i < this->maxRecentProjects; i++)
                   {
                      RecentProject recent;
                      Serializer::Deserialize::string(stream, recent.id);
                     Serializer::Deserialize::string(stream, recent.name);
                     Serializer::Deserialize::string(stream, recent.directory);
                     this->recentProjects.push_back(recent);
                   } });
}

bool ProjectManager::isReady()
{
  if (this->data.directory.empty())
    return false;
  if (this->data.name.empty())
    return false;
  return true;
}

void ProjectManager::serialize(bool create)
{
  std::filesystem::path fullPath = std::filesystem::path(this->data.directory) / this->projectFileName;
  if (create)
    this->data.id = this->generateUID();

  Serializer::out(fullPath, [this](auto &stream)
                  {
    Serializer::Serialize::string(stream, this->data.id);
    Serializer::Serialize::string(stream, this->data.name);
    Serializer::Serialize::string(stream, this->data.directory); });
}

void ProjectManager::deserialize(const std::string &directory)
{
  std::filesystem::path fullPath = std::filesystem::path(directory) / this->projectFileName;

  Serializer::in(fullPath, [this](auto &stream)
                 {
    Serializer::Deserialize::string(stream, this->data.id);
    Serializer::Deserialize::string(stream, this->data.name);
    Serializer::Deserialize::string(stream, this->data.directory); });
}

const char *ProjectManager::getProjectFileName()
{
  return this->projectFileName.c_str();
}

void ProjectManager::updateRecentProjects()
{
  std::filesystem::path recentFilePath = std::filesystem::current_path() / this->recentFileName;

  auto it = std::remove_if(this->recentProjects.begin(), this->recentProjects.end(), [this](const RecentProject &recent)
                           { return recent.id == this->data.id; });

  this->recentProjects.erase(it, this->recentProjects.end());

  this->recentProjects.emplace_back<RecentProject>({this->data.id, this->data.name, this->data.directory});
  if (this->recentProjects.size() > this->maxRecentProjects)
    this->recentProjects.erase(this->recentProjects.begin(), this->recentProjects.begin() + (this->recentProjects.size() - this->maxRecentProjects));

  Serializer::out(recentFilePath, [this](auto &stream)
                 {
    for (RecentProject &recent : this->recentProjects)
    {
      Serializer::Serialize::string(stream, recent.id);
      Serializer::Serialize::string(stream, recent.name);
      Serializer::Serialize::string(stream, recent.directory);
    } });
}

std::string ProjectManager::generateUID()
{
  auto now = std::chrono::high_resolution_clock::now();
  auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(100000, 999999);

  int randomPart = dis(gen);

  std::stringstream ss;
  ss << timestamp << "_" << randomPart;

  return ss.str();
}
