#pragma once

#include "imgui.h"

#include <array>
#include <cstdlib>
#include <iostream>
#include <pwd.h>
#include <string>
#include <unistd.h>

#ifdef DEBUG

#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <glm/glm.hpp>
#include <immintrin.h>
#include <thread>

namespace fs = std::filesystem;

template <typename... Args>
inline void Log(const char *file, int line, const char *functionName,
                const Args &...args) {
  std::cout << "LOG " << file << ":" << line << " (" << functionName << "):";
  ((std::cout << " " << args), ...);
  std::cout << std::endl;
}

inline void LogIVec3(const char *file, int line, const char *functionName,
                     const std::string &name, const glm::ivec3 &position) {

  Log(file, line, functionName, name, "(", position.x, ",", position.y, ",",
      position.z, ")");
}

inline void LogVec3(const char *file, int line, const char *functionName,
                    const std::string &name, const glm::vec3 &position) {

  Log(file, line, functionName, name, "(", position.x, ",", position.y, ",",
      position.z, ")");
}

inline void LogVec4(const char *file, int line, const char *functionName,
                    const std::string &name, const glm::vec4 &position) {

  Log(file, line, functionName, name, "(", position.x, ",", position.y, ",",
      position.z, ",", position.w, ")");
}

template <typename... Args>
inline void LogToFile(const char *file, int line, const char *functionName,
                      const std::string &outputFile, const Args &...args) {
  const std::string logDir = "logs/";
  fs::create_directories(logDir);

  std::ofstream ofs(logDir + outputFile, std::ios::app);
  if (!ofs.is_open())
    return;

  ofs << "LOG " << file << ":" << line << " (" << functionName << "):";
  ((ofs << " " << args), ...);
  ofs << std::endl;
}

inline void Benchmark(const char *file, int line, const char *functionName,
                      const std::function<void()> &func, int iterations) {

  for (int i = 0; i < 50; ++i)
    func();

  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < iterations; ++i)
    func();

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;

  Log(file, line, functionName, "Took:", elapsed.count() / iterations,
      "ms (average) over", iterations, "iterations");
}

template <typename Tuple, size_t... I>
void StreamTextArgs(const Tuple &tuple, std::stringstream &ss,
                    std::index_sequence<I...>) {
  (void)std::initializer_list<int>{(ss << std::get<I>(tuple), 0)...};
}

template <typename... Args>
inline void EndTimer(const char *file, int line, const char *functionName,
                     Args &&...args) {
  static_assert(sizeof...(Args) >= 1,
                "EndTimer requires at least one argument (startTime)");

  constexpr size_t N = sizeof...(Args);
  auto tuple = std::make_tuple(std::forward<Args>(args)...);

  // Last argument = startTime
  const auto &startTime = std::get<N - 1>(tuple);

  std::stringstream ss;
  StreamTextArgs(tuple, ss, std::make_index_sequence<N - 1>{});

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> duration = end - startTime;

  Log(file, line, functionName, ss.str(), duration.count(), "ms");
}

#define LOG(...) Log(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define BENCHMARK(...) Benchmark(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define START_TIMER std::chrono::high_resolution_clock::now()
#define END_TIMER(...) EndTimer(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_IVEC3(...) LogIVec3(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_VEC3(...) LogVec3(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_VEC4(...) LogVec4(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_TO_FILE(outputFile, ...)                                           \
  LogToFile(__FILE__, __LINE__, __func__, outputFile, __VA_ARGS__)

#else
#define LOG(...)
#define BENCHMARK(...)
#define START_TIMER 0
#define END_TIMER(...)
#define LOG_IVEC3(...)
#define LOG_VEC3(...)
#define LOG_VEC4(...)
#define LOG_TO_FILE(...)
#endif

static std::string GetHomeDirectory() {
  const char *home = std::getenv("HOME");
  if (home)
    return home;
  struct passwd *pw = getpwuid(getuid());
  return pw ? pw->pw_dir : "";
}

static std::string GetExecutableDirectory() {
  char result[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
  return std::filesystem::path(
             std::string(result,
                         (count > 0) ? static_cast<unsigned int>(count) : 0))
      .parent_path()
      .string();
}

enum class EllipsizeType { START, END };

static std::string Ellipsize(const std::string &text, float maxWidth,
                             EllipsizeType type = EllipsizeType::END) {
  std::string truncatedText = text;

  ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

  if (textSize.x <= maxWidth)
    return truncatedText;

  constexpr const char *ellipsis = "...";
  float availableWidth = maxWidth - ImGui::CalcTextSize(ellipsis).x;

  size_t length = text.length() - 1;

  if (type == EllipsizeType::START) {
    while (ImGui::CalcTextSize(text.substr(length).c_str()).x < availableWidth)
      --length;

    truncatedText = ellipsis + text.substr(length);
  }

  if (type == EllipsizeType::END) {
    while (ImGui::CalcTextSize(text.substr(0, length).c_str()).x >
           availableWidth)
      --length;

    truncatedText = text.substr(0, length) + ellipsis;
  }

  return truncatedText;
}

static std::string ExecCommand(const char *cmd) {
  std::array<char, 128> buffer;
  std::string result;
  FILE *pipe = popen(cmd, "r");
  if (!pipe)
    return "";
  while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
    result += buffer.data();
  pclose(pipe);
  return result;
}

static bool IsDarkMode() {
  std::string theme =
      ExecCommand("gsettings get org.gnome.desktop.interface gtk-theme");
  return theme.find("dark") != std::string::npos;
}

static std::string RelativeHomePath(const std::filesystem::path &p) {
  const char *home = std::getenv("HOME");
  if (!home)
    return p.string();

  std::filesystem::path homePath(home);

  // check if `p` starts with home
  auto abs = std::filesystem::absolute(p);
  if (abs.string().rfind(homePath.string(), 0) == 0) {
    return "~" + abs.string().substr(homePath.string().size());
  }

  return abs.string();
}

static std::string AddFileExtension(const std::string &filepath,
                                    const std::string &extension) {
  std::filesystem::path p(filepath);
  if (p.extension() != extension)
    p.replace_extension(extension);
  return p.string();
}