#include "MbedLittleFSWrapper.h"

#include <String.h>
#include <Arduino.h>

#include "FS.h"
#include "LittleFS.h"

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

bool MbedLittleFSWrapper::is_symlink(const char *path) {
    int err = lfs_issymlink(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::is_readonly(const char *path) {
    int err = lfs_isreadonly(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::is_hidden(const char *path) {
    int err = lfs_ishidden(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::is_writable(const char *path) {
    int err = lfs_iswritable(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::is_readable(const char *path) {
    int err = lfs_isreadable(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::is_executable(const char *path) {
    int err = lfs_isexecutable(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::is_deletable(const char *path) {
    int err = lfs_isdeletable(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::is_updatable(const char *path) {
    int err = lfs_isupdatable(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::is_sequential(const char *path) {
    int err = lfs_issequential(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::is_random(const char *path) {
    int err = lfs_israndom(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::is_appendable(const char *path) {
    int err = lfs_isappendable(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::is_truncatable(const char *path) {
    int err = lfs_istruncatable(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::is_seekable(const char *path) {
    int err = lfs_isseekable(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::is_findable(const char *path) {
    int err = lfs_isfindable(&_lfs, path);
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

bool MbedLittleFSWrapper::opendir(const char *path) {
    int err = lfs_opendir(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::readdir(const char *path) {
    int err = lfs_readdir(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::closedir(const char *path) {
    int err = lfs_closedir(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::open(const char *path, const char *mode) {
    int err = lfs_open(&_lfs, path, mode);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::close(const char *path) {
    int err = lfs_close(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::read(const char *path, void *buffer, size_t size) {
    int err = lfs_read(&_lfs, path, buffer, size);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::write(const char *path, const void *buffer, size_t size) {
    int err = lfs_write(&_lfs, path, buffer, size);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::seek(const char *path, off_t offset, int whence) {
    int err = lfs_seek(&_lfs, path, offset, whence);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::tell(const char *path) {
    int err = lfs_tell(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::rewind(const char *path) {
    int err = lfs_rewind(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::eof(const char *path) {
    int err = lfs_eof(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::truncate(const char *path, off_t length) {
    int err = lfs_truncate(&_lfs, path, length);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::sync(const char *path) {
    int err = lfs_sync(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::size(const char *path) {
    int err = lfs_size(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::free(const char *path) {
    int err = lfs_free(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::stat(const char *path) {
    int err = lfs_stat(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::lstat(const char *path) {
    int err = lfs_lstat(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::fstat(const char *path) {
    int err = lfs_fstat(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::link(const char *old_path, const char *new_path) {
    int err = lfs_link(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::symlink(const char *old_path, const char *new_path) {
    int err = lfs_symlink(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::readlink(const char *path, char *buffer, size_t size) {
    int err = lfs_readlink(&_lfs, path, buffer, size);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::chmod(const char *path, mode_t mode) {
    int err = lfs_chmod(&_lfs, path, mode);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::chown(const char *path, uid_t owner, gid_t group) {
    int err = lfs_chown(&_lfs, path, owner, group);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::utime(const char *path, const struct utimbuf *times) {
    int err = lfs_utime(&_lfs, path, times);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::realpath(const char *path, char *resolved_path) {
    int err = lfs_realpath(&_lfs, path, resolved_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::remove_recursive(const char *path) {
    int err = lfs_remove_recursive(&_lfs, path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy(const char *old_path, const char *new_path) {
    int err = lfs_copy(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move(const char *old_path, const char *new_path) {
    int err = lfs_move(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_recursive(const char *old_path, const char *new_path) {
    int err = lfs_copy_recursive(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move_recursive(const char *old_path, const char *new_path) {
    int err = lfs_move_recursive(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_file(const char *old_path, const char *new_path) {
    int err = lfs_copy_file(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move_file(const char *old_path, const char *new_path) {
    int err = lfs_move_file(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_directory(const char *old_path, const char *new_path) {
    int err = lfs_copy_directory(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move_directory(const char *old_path, const char *new_path) {
    int err = lfs_move_directory(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_symlink(const char *old_path, const char *new_path) {
    int err = lfs_copy_symlink(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move_symlink(const char *old_path, const char *new_path) {
    int err = lfs_move_symlink(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_file_recursive(const char *old_path, const char *new_path) {
    int err = lfs_copy_file_recursive(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move_file_recursive(const char *old_path, const char *new_path) {
    int err = lfs_move_file_recursive(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_directory_recursive(const char *old_path, const char *new_path) {
    int err = lfs_copy_directory_recursive(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move_directory_recursive(const char *old_path, const char *new_path) {
    int err = lfs_move_directory_recursive(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_symlink_recursive(const char *old_path, const char *new_path) {
    int err = lfs_copy_symlink_recursive(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move_symlink_recursive(const char *old_path, const char *new_path) {
    int err = lfs_move_symlink_recursive(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_file_to_directory(const char *old_path, const char *new_path) {
    int err = lfs_copy_file_to_directory(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move_file_to_directory(const char *old_path, const char *new_path) {
    int err = lfs_move_file_to_directory(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_directory_to_directory(const char *old_path, const char *new_path) {
    int err = lfs_copy_directory_to_directory(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move_directory_to_directory(const char *old_path, const char *new_path) {
    int err = lfs_move_directory_to_directory(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_symlink_to_directory(const char *old_path, const char *new_path) {
    int err = lfs_copy_symlink_to_directory(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move_symlink_to_directory(const char *old_path, const char *new_path) {
    int err = lfs_move_symlink_to_directory(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_file_to_directory_recursive(const char *old_path, const char *new_path) {
    int err = lfs_copy_file_to_directory_recursive(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move_file_to_directory_recursive(const char *old_path, const char *new_path) {
    int err = lfs_move_file_to_directory_recursive(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_directory_to_directory_recursive(const char *old_path, const char *new_path) {
    int err = lfs_copy_directory_to_directory_recursive(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move_directory_to_directory_recursive(const char *old_path, const char *new_path) {
    int err = lfs_move_directory_to_directory_recursive(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_symlink_to_directory_recursive(const char *old_path, const char *new_path) {
    int err = lfs_copy_symlink_to_directory_recursive(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move_symlink_to_directory_recursive(const char *old_path, const char *new_path) {
    int err = lfs_move_symlink_to_directory_recursive(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_file_to_directory(const char *old_path, const char *new_path) {
    int err = lfs_copy_file_to_directory(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move_file_to_directory(const char *old_path, const char *new_path) {
    int err = lfs_move_file_to_directory(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_directory_to_directory(const char *old_path, const char *new_path) {
    int err = lfs_copy_directory_to_directory(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move_directory_to_directory(const char *old_path, const char *new_path) {
    int err = lfs_move_directory_to_directory(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_symlink_to_directory(const char *old_path, const char *new_path) {
    int err = lfs_copy_symlink_to_directory(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move_symlink_to_directory(const char *old_path, const char *new_path) {
    int err = lfs_move_symlink_to_directory(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_file_to_directory_recursive(const char *old_path, const char *new_path) {
    int err = lfs_copy_file_to_directory_recursive(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move_file_to_directory_recursive(const char *old_path, const char *new_path) {
    int err = lfs_move_file_to_directory_recursive(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_directory_to_directory_recursive(const char *old_path, const char *new_path) {
    int err = lfs_copy_directory_to_directory_recursive(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move_directory_to_directory_recursive(const char *old_path, const char *new_path) {
    int err = lfs_move_directory_to_directory_recursive(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_symlink_to_directory_recursive(const char *old_path, const char *new_path) {
    int err = lfs_copy_symlink_to_directory_recursive(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move_symlink_to_directory_recursive(const char *old_path, const char *new_path) {
    int err = lfs_move_symlink_to_directory_recursive(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_file_to_directory(const char *old_path, const char *new_path) {
    int err = lfs_copy_file_to_directory(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move_file_to_directory(const char *old_path, const char *new_path) {
    int err = lfs_move_file_to_directory(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_directory_to_directory(const char *old_path, const char *new_path) {
    int err = lfs_copy_directory_to_directory(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::move_directory_to_directory(const char *old_path, const char *new_path) {
    int err = lfs_move_directory_to_directory(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

bool MbedLittleFSWrapper::copy_symlink_to_directory(const char *old_path, const char *new_path) {
    int err = lfs_copy_symlink_to_directory(&_lfs, old_path, new_path);
    if (err < 0) {
        return false;
    }

    return true;
}

