#include "MbedLittleFSWrapper.h"

#include <String.h>
#include <Arduino.h>

#include "LFS.h"
#include "lfs_util.h"

extern MbedLittleFSWrapper LittleFS;

bool MbedLittleFSWrapper::mount() {
    if (_mounted) {
        return true;
    }

    int err = lfs_mount(&_lfs, &_cfg);
    if (err < 0) {
        return false;
    }

    _mounted = true;
    return true;
}

bool MbedLittleFSWrapper::unmount() {
    if (!_mounted) {
        return true;
    }

    int err = lfs_unmount(&_lfs);
    if (err < 0) {
        return false;
    }

    _mounted = false;
    return true;
}

bool MbedLittleFSWrapper::format() {
    int err = lfs_format(&_lfs, &_cfg);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::remove(const char *path) {
    int err = lfs_remove(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::rename(const char *old_path, const char *new_path) {
    int err = lfs_rename(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::exists(const char *path) {
    int err = lfs_exists(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::is_directory(const char *path) {
    int err = lfs_isdir(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::is_file(const char *path) {
    int err = lfs_isfile(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}


bool MbedLittleFSWrapper::mkdir(const char *path) {
    int err = lfs_mkdir(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::rmdir(const char *path) {
    int err = lfs_rmdir(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}


bool MbedLittleFSWrapper::stat(const char *path) {
    struct lfs_info info;
    int err = lfs_stat(&_lfs, path, &info);
    if (err < 0) {
        return false;
    }

    return true;
}



