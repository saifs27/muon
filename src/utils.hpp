#pragma once

enum class FileError {
    FileNotFound,
    ReadFailed,
    OpenFailed,
    SeekFailed,
    UnexpectedData,
};
