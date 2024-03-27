#ifndef MBED_LITTLEFS_WRAPPER_H
#define MBED_LITTLEFS_WRAPPER_H



class MbedLittleFSWrapper {
    bool _mounted;
    lfs_t _lfs;
    lfs_config _cfg;

public:
    bool mount(); // mount the filesystem
    bool unmount(); // unmount the filesystem
    bool format(); // format the filesystem
    bool remove(const char *path); // remove a file or directory
    bool rename(const char *old_path, const char *new_path); // rename a file or directory
    bool exists(const char *path); // check if a file or directory exists
    bool is_directory(const char *path); // check if a path is a directory
    bool is_file(const char *path); // check if a path is a file
    bool is_symlink(const char *path); // check if a path is a symlink
    bool is_readonly(const char *path); // check if a path is readonly
    bool is_hidden(const char *path); // check if a path is hidden
    bool is_writable(const char *path); // check if a path is writable
    bool is_readable(const char *path); // check if a path is readable
    bool is_executable(const char *path); // check if a path is executable
    bool is_deletable(const char *path); // check if a path is deletable
    bool is_updatable(const char *path); // check if a path is updatable
    bool is_sequential(const char *path); // check if a path is sequential
    bool is_random(const char *path); // check if a path is random
    bool is_appendable(const char *path); // check if a path is appendable
    bool is_truncatable(const char *path); // check if a path is truncatable
    bool is_seekable(const char *path); // check if a path is seekable
    bool is_findable(const char *path); // check if a path is findable
    bool mkdir(const char *path); // create a directory
    bool rmdir(const char *path); // remove a directory
    bool opendir(const char *path); // open a directory
    bool readdir(const char *path); // read a directory
    bool closedir(const char *path); // close a directory
    bool open(const char *path, const char *mode); // open a file
    bool close(const char *path); // close a file
    bool read(const char *path, void *buffer, size_t size); // read from a file
    bool write(const char *path, const void *buffer, size_t size); // write to a file
    bool seek(const char *path, off_t offset, int whence); // seek in a file
    bool tell(const char *path); // tell the position in a file
    bool rewind(const char *path); // rewind the position in a file
    bool eof(const char *path); // check if the end of a file has been reached
    bool truncate(const char *path, off_t length); // truncate a file
    bool sync(const char *path); // sync a file
    bool size(const char *path); // get the size of a file
    bool free(const char *path); // get the free space on the filesystem
    bool stat(const char *path); // get the status of a file
    bool lstat(const char *path); // get the status of a file
    bool fstat(const char *path); // get the status of a file
    bool link(const char *old_path, const char *new_path); // create a hard link
    bool symlink(const char *old_path, const char *new_path); // create a symbolic link
    bool readlink(const char *path, char *buffer, size_t size); // read a symbolic link
    bool chmod(const char *path, mode_t mode); // change the mode of a file
    bool chown(const char *path, uid_t owner, gid_t group); // change the owner and group of a file
    bool utime(const char *path, const struct utimbuf *times); // change the access and modification times of a file
    bool realpath(const char *path, char *resolved_path); // resolve a path
    bool remove_recursive(const char *path); // remove a file or directory recursively
    bool copy(const char *old_path, const char *new_path); // copy a file or directory
    bool move(const char *old_path, const char *new_path); // move a file or directory
    bool copy_recursive(const char *old_path, const char *new_path); // copy a file or directory recursively
    bool move_recursive(const char *old_path, const char *new_path); // move a file or directory recursively
    bool copy_file(const char *old_path, const char *new_path); // copy a file
    bool move_file(const char *old_path, const char *new_path); // move a file
    bool copy_directory(const char *old_path, const char *new_path); // copy a directory
    bool move_directory(const char *old_path, const char *new_path); // move a directory
    bool copy_symlink(const char *old_path, const char *new_path); // copy a symlink
    bool move_symlink(const char *old_path, const char *new_path); // move a symlink
    bool copy_file_recursive(const char *old_path, const char *new_path); // copy a file recursively
    bool move_file_recursive(const char *old_path, const char *new_path); // move a file recursively
    bool copy_directory_recursive(const char *old_path, const char *new_path); // copy a directory recursively
    bool move_directory_recursive(const char *old_path, const char *new_path);  // move a directory recursively
    bool copy_symlink_recursive(const char *old_path, const char *new_path); // copy a symlink recursively
    bool move_symlink_recursive(const char *old_path, const char *new_path); // move a symlink recursively
    bool copy_file_to_directory(const char *old_path, const char *new_path); // copy a file to a directory
    bool move_file_to_directory(const char *old_path, const char *new_path); // move a file to a directory
    bool copy_directory_to_directory(const char *old_path, const char *new_path); // copy a directory to a directory
    bool move_directory_to_directory(const char *old_path, const char *new_path); // move a directory to a directory
    bool copy_symlink_to_directory(const char *old_path, const char *new_path); // copy a symlink to a directory
    bool move_symlink_to_directory(const char *old_path, const char *new_path); // move a symlink to a directory
    bool copy_file_to_directory_recursive(const char *old_path, const char *new_path); // copy a file to a directory recursively
    bool move_file_to_directory_recursive(const char *old_path, const char *new_path); // move a file to a directory recursively
    bool copy_directory_to_directory_recursive(const char *old_path, const char *new_path); // copy a directory to a directory recursively
    bool move_directory_to_directory_recursive(const char *old_path, const char *new_path); // move a directory to a directory recursively
    bool copy_symlink_to_directory_recursive(const char *old_path, const char *new_path); // copy a symlink to a directory recursively
    bool move_symlink_to_directory_recursive(const char *old_path, const char *new_path); // move a symlink to a directory recursively
    bool copy_file_to_directory(const char *old_path, const char *new_path); // copy a file to a directory
    bool move_file_to_directory(const char *old_path, const char *new_path); // move a file to a directory
 
    








};

#endif //MBED_LITTLEFS_WRAPPER_H