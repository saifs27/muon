#include "memory_map.hpp"

#include <array>
#include <bit>
#include <cstdint>
#include <utility>

#if defined(_WIN32)
    #include <windows.h>
#else
    #include <fcntl.h>
    #include <sys/mman.h>
    #include <unistd.h>
#endif

std::expected<MemoryMap, FileError> MemoryMap::map(const std::filesystem::path& file_path) {
    std::error_code ec;
    void * mapped = nullptr;
    size_t length = 0;
    if(!std::filesystem::exists(file_path, ec)) {
        return std::unexpected(FileError::FileNotFound);
    }

    auto file_size = std::filesystem::file_size(file_path, ec);
    if (file_size == 0) {
        return std::unexpected(FileError::FileEmpty);
    }

    length = file_size;

    #if defined(_WIN32)
        auto path = file_path.native();
        HANDLE hFile = CreateFileW(
            path.c_str(), 
            GENERIC_READ, 
            FILE_SHARE_READ, 
            nullptr, 
            OPEN_EXISTING, 
            FILE_ATTRIBUTE_NORMAL, 
            nullptr);

        if (hFile == INVALID_HANDLE_VALUE) {
            return std::unexpected(FileError::OpenFailed);
        }

        HANDLE hMap = CreateFileMapping(
            hFile, 
            nullptr, 
            PAGE_READONLY, 
            0, 
            0, 
            nullptr);

        CloseHandle(hFile);
        if (!hMap) {
            return std::unexpected(FileError::MappingFailed);
        }

        mapped = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
        CloseHandle(hMap);
    #else
        auto path = file_path.native()
        int fd = open(path.c_str(), O_RDONLY);
        if (fd == -1) {
            return std::unexpected(FileError::OpenFailed);
        }

        mapped = mmap(nullptr, length, PROT_READ, MAP_PRIVATE, fd, 0);
        close(fd);

        if (mapped == MAP_FAILED) {
            mapped = nullptr;
            return std::unexpected(FileError::MappingFailed);
        }

    #endif

    return MemoryMap(M{
        .data = mapped,
        .length = length,
    });
}

void MemoryMap::unmap() {

    if (m.data == nullptr || m.length == 0) return;

    #if defined (_WIN32)
        UnmapViewOfFile(m.data);
    #else
        munmap(m.data, m.length);
    #endif

    m.data = nullptr;
    m.length = 0;
}

MemoryMap::MemoryMap(MemoryMap&& other) noexcept
    : m(std::exchange(other.m, M{.data = nullptr, .length = 0}))
    {}

MemoryMap& MemoryMap::operator=(MemoryMap&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    unmap();
    m = std::exchange(other.m, M{.data = nullptr, .length = 0});
    return *this;
}