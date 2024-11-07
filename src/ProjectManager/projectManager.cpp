#include "projectManager.h"
#include "BinaryIO/binaryIO.h"

ProjectManager::ProjectManager(std::string fileExtension, std::string recentFileName, int maxRecentProjects) : fileExtension(fileExtension), recentFileName(recentFileName), maxRecentProjects(maxRecentProjects)
{
  std::filesystem::path recentFilePath = std::filesystem::current_path() / this->recentFileName;

  BinaryIO::in(recentFilePath, [this](auto &stream)
               {
    for (size_t i = 0; i < this->maxRecentProjects; i++)
    {
      RecentProject recent;
      bool success = BinaryIO::Read::string(stream, recent.id);
      if(!success) break;
      BinaryIO::Read::string(stream, recent.name);
      BinaryIO::Read::string(stream, recent.filepath);
      this->recentProjects.push_back(recent);
    } });
}

bool ProjectManager::isReady()
{
  if (this->data.filepath.empty())
    return false;
  if (this->data.name.empty())
    return false;
  return true;
}

void ProjectManager::serialize(bool create)
{
  if (create)
    this->data.id = this->generateUID();

  BinaryIO::out(this->data.filepath, [this](auto &stream)
                {
    BinaryIO::Write::string(stream, this->data.id);
    BinaryIO::Write::string(stream, this->data.name);
    BinaryIO::Write::string(stream, this->data.filepath); });
}

void ProjectManager::deserialize(const std::string &filepath)
{
  BinaryIO::in(filepath, [this](auto &stream)
               {
    BinaryIO::Read::string(stream, this->data.id);
    BinaryIO::Read::string(stream, this->data.name);
    BinaryIO::Read::string(stream, this->data.filepath); });
}

const char *ProjectManager::getFileExtension()
{
  return this->fileExtension.c_str();
}

void ProjectManager::updateRecentProjects()
{
  std::filesystem::path recentFilePath = std::filesystem::current_path() / this->recentFileName;

  auto it = std::remove_if(this->recentProjects.begin(), this->recentProjects.end(), [this](const RecentProject &recent)
                           { return recent.id == this->data.id; });

  if (it != this->recentProjects.end())
    this->recentProjects.erase(it);

  this->recentProjects.emplace(this->recentProjects.begin(), this->data.id, this->data.name, this->data.filepath);

  if (this->recentProjects.size() > this->maxRecentProjects)
    this->recentProjects.erase(this->recentProjects.begin() + this->maxRecentProjects, this->recentProjects.end());

  BinaryIO::out(recentFilePath, [this](auto &stream)
                {
    for (RecentProject &recent : this->recentProjects)
    {
      BinaryIO::Write::string(stream, recent.id);
      BinaryIO::Write::string(stream, recent.name);
      BinaryIO::Write::string(stream, recent.filepath);
    } });
}

void ProjectManager::quit()
{
  this->isRunning = false;
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
