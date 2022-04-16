/**
 * @file lv_port_fs_templ.c
 *
 */

#include "bsp.h"

#ifdef USE_LVGL

//-------------------------------------------------------------------------------------------------
// INCLUDES
//-------------------------------------------------------------------------------------------------
 
#include "lv_port_fs.h"

#ifdef USE_YAFFS2
#include "../../yaffs2/direct/yportenv.h"
#include "../../yaffs2/direct/yaffsfs.h"
extern int yaffs_startup_and_mount(const char *mount_name);
#endif

//-------------------------------------------------------------------------------------------------
// DEFINES
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// TYPEDEFS
//-------------------------------------------------------------------------------------------------

/* Create a type to store the required data about your file.
 * If you are using a File System library
 * it already should have a File type.
 * For example FatFS has `FIL`. In this case use `typedef FIL file_t`
 */
typedef struct
{
    /* Add the data you need to store about a file */
    int handle;
    int dummy;
} file_t;

/* Similarly to `file_t` create a type for directory reading too
 */
typedef struct
{
    /* Add the data you need to store about directory reading */
    unsigned int handle;        // (unsigned int)(yaffs_DIR *)
    int dummy;
} dir_t;

//-------------------------------------------------------------------------------------------------
// STATIC PROTOTYPES
//-------------------------------------------------------------------------------------------------

static void fs_init(void);

static lv_fs_res_t fs_open(lv_fs_drv_t *drv, void *file_p, const char *path, lv_fs_mode_t mode);
static lv_fs_res_t fs_close(lv_fs_drv_t *drv, void *file_p);
static lv_fs_res_t fs_read(lv_fs_drv_t *drv, void *file_p, void *buf, unsigned int btr, unsigned int *br);
static lv_fs_res_t fs_write(lv_fs_drv_t *drv, void *file_p, const void *buf, unsigned int btw, unsigned int *bw);
static lv_fs_res_t fs_seek(lv_fs_drv_t *drv, void *file_p, unsigned int pos);
static lv_fs_res_t fs_size(lv_fs_drv_t *drv, void *file_p, unsigned int *size_p);
static lv_fs_res_t fs_tell(lv_fs_drv_t *drv, void *file_p, unsigned int *pos_p);
static lv_fs_res_t fs_remove(lv_fs_drv_t *drv, const char *path);
static lv_fs_res_t fs_trunc(lv_fs_drv_t *drv, void *file_p);
static lv_fs_res_t fs_rename(lv_fs_drv_t *drv, const char *oldname, const char *newname);
static lv_fs_res_t fs_free(lv_fs_drv_t *drv, unsigned int *total_p, unsigned int *free_p);
static lv_fs_res_t fs_dir_open(lv_fs_drv_t *drv, void *rddir_p, const char *path);
static lv_fs_res_t fs_dir_read(lv_fs_drv_t *drv, void *rddir_p, char *fn);
static lv_fs_res_t fs_dir_close(lv_fs_drv_t *drv, void *rddir_p);

//-------------------------------------------------------------------------------------------------
// STATIC VARIABLES
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// GLOBAL PROTOTYPES
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// MACROS
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// GLOBAL FUNCTIONS
//-------------------------------------------------------------------------------------------------

void lv_port_fs_init(void)
{
    /*-----------------------------------------------------
     * Initialize your storage device and File System
     * ---------------------------------------------------*/
     
    fs_init();

    /*-----------------------------------------------------
     * Register the file system interface  in LVGL
     *----------------------------------------------------*/

    /* Add a simple drive to open images */
    lv_fs_drv_t fs_drv;
    
    lv_fs_drv_init(&fs_drv);

    /* Set up fields...*/
    fs_drv.file_size     = sizeof(file_t);
    fs_drv.letter        = 'P';
    fs_drv.open_cb       = fs_open;
    fs_drv.close_cb      = fs_close;
    fs_drv.read_cb       = fs_read;
    fs_drv.write_cb      = fs_write;
    fs_drv.seek_cb       = fs_seek;
    fs_drv.tell_cb       = fs_tell;
    fs_drv.free_space_cb = fs_free;
    fs_drv.size_cb       = fs_size;
    fs_drv.remove_cb     = fs_remove;
    fs_drv.rename_cb     = fs_rename;
    fs_drv.trunc_cb      = fs_trunc;

    fs_drv.rddir_size    = sizeof(dir_t);
    fs_drv.dir_close_cb  = fs_dir_close;
    fs_drv.dir_open_cb   = fs_dir_open;
    fs_drv.dir_read_cb   = fs_dir_read;

    lv_fs_drv_register(&fs_drv);
}

//-------------------------------------------------------------------------------------------------
// STATIC FUNCTIONS
//-------------------------------------------------------------------------------------------------

/*
 * change errno TO lvgl_fs_error
 */
static lv_fs_res_t errno_to_lvgl_error(void)
{
#ifdef USE_YAFFS2
    int err = yaffsfs_GetLastError();
    
    switch (err)
    {
        case 0:
            return LV_FS_RES_OK;
            
        case ENODEV:
        case ENOENT:
            return LV_FS_RES_NOT_EX;        /* Driver, file or directory is not exists */
        
        case ENOSPC:
            return LV_FS_RES_FULL;          /* Disk full */
            
        case EACCES:
            return LV_FS_RES_DENIED;        /* Access denied. Check 'fs_open' modes and write protect */
            
        case EBUSY:
            return LV_FS_RES_BUSY;          /* The file system now can't handle it, try later */

        case ENOMEM:
            return LV_FS_RES_OUT_OF_MEM;    /* Not enough memory for an internal operation */
            
        case EINVAL:
            return LV_FS_RES_INV_PARAM;     /* Invalid parameter among arguments */

        default:
            return LV_FS_RES_UNKNOWN;       /* Other unknown error */
    }
    
#else
    return LV_FS_RES_UNKNOWN;
    
#endif
}

/*
 * Initialize your Storage device and File system.
 */
static void fs_init(void)
{
    /* E.g. for FatFS initalize the SD card and FatFS itself */

#ifdef USE_YAFFS2
    yaffs_startup_and_mount("/ndd");    // YAFFS_MOUNTED_FS_NAME
#endif

}

/*
 * Open a file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param path path to the file beginning with the driver letter (e.g. S:/folder/file.txt)
 * @param mode read: FS_MODE_RD, write: FS_MODE_WR, both: FS_MODE_RD | FS_MODE_WR
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_open(lv_fs_drv_t *drv, void *file_p, const char *path, lv_fs_mode_t mode)
{
#ifdef USE_YAFFS2
    file_t *fp = (file_t *)file_p;
    
    if (fp->handle)
        return LV_FS_RES_LOCKED;            /* The file is already opened */

    if (mode == LV_FS_MODE_WR)              /* Open a file for write */
    {
        fp->handle = yaffs_open(path, O_CREAT | O_WRONLY, 0777);
    }
    else if (mode == LV_FS_MODE_RD)         /* Open a file for read */
    {
        fp->handle = yaffs_open(path, O_CREAT | O_RDONLY, 0777);
    }
    else if (mode == (LV_FS_MODE_WR | LV_FS_MODE_RD))   /* Open a file for read and write */
    {
        fp->handle = yaffs_open(path, O_CREAT | O_RDWR, 0777);
    }
    else    // TODO file open default mode
    {
        fp->handle = 0;
        return LV_FS_RES_FS_ERR;
    }

    return errno_to_lvgl_error();
    
#else
    return LV_FS_RES_NOT_IMP;
    
#endif
}

/*
 * Close an opened file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable. (opened with lv_ufs_open)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_close(lv_fs_drv_t *drv, void *file_p)
{
#ifdef USE_YAFFS2
    file_t *fp = (file_t *)file_p;
    
    yaffs_close(fp->handle);
    fp->handle = 0;
    
    return errno_to_lvgl_error();
    
#else
    return LV_FS_RES_NOT_IMP;
    
#endif
}

/*
 * Read data from an opened file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable.
 * @param buf pointer to a memory block where to store the read data
 * @param btr number of Bytes To Read
 * @param br the real number of read bytes (Byte Read)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_read(lv_fs_drv_t *drv, void *file_p, void *buf, unsigned int btr, unsigned int *br)
{
#ifdef USE_YAFFS2
    file_t *fp = (file_t *)file_p;

    *br = yaffs_read(fp->handle, buf, btr);

    return errno_to_lvgl_error();

#else
    return LV_FS_RES_NOT_IMP;

#endif
}

/*
 * Write into a file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param buf pointer to a buffer with the bytes to write
 * @param btr Bytes To Write
 * @param br the number of real written bytes (Bytes Written). NULL if unused.
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_write(lv_fs_drv_t *drv, void *file_p, const void *buf, unsigned int btw, unsigned int *bw)
{
#ifdef USE_YAFFS2
    file_t *fp = (file_t *)file_p;

    *bw = yaffs_write(fp->handle, buf, btw);
    
    return errno_to_lvgl_error();

#else
    return LV_FS_RES_NOT_IMP;

#endif
}

/*
 * Set the read write pointer. Also expand the file size if necessary.
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable. (opened with lv_ufs_open )
 * @param pos the new position of read write pointer
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_seek(lv_fs_drv_t *drv, void *file_p, unsigned int pos)
{
#ifdef USE_YAFFS2
    file_t *fp = (file_t *)file_p;

    yaffs_lseek(fp->handle, pos, SEEK_SET);

    return errno_to_lvgl_error();

#else
    return LV_FS_RES_NOT_IMP;

#endif
}

/*
 * Give the size of a file bytes
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param size pointer to a variable to store the size
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_size(lv_fs_drv_t *drv, void *file_p, unsigned int *size_p)
{
#ifdef USE_YAFFS2
    file_t *fp = (file_t *)file_p;

    *size_p = yaffs_lseek(fp->handle, 0, SEEK_END);

    return errno_to_lvgl_error();

#else
    return LV_FS_RES_NOT_IMP;

#endif
}

/*
 * Give the position of the read write pointer
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable.
 * @param pos_p pointer to to store the result
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_tell(lv_fs_drv_t *drv, void *file_p, unsigned int *pos_p)
{
#ifdef USE_YAFFS2
    file_t *fp = (file_t *)file_p;

    *pos_p = yaffs_lseek(fp->handle, 0, SEEK_CUR);
    
    return errno_to_lvgl_error();

#else
    return LV_FS_RES_NOT_IMP;

#endif
}

/*
 * Delete a file
 * @param drv pointer to a driver where this function belongs
 * @param path path of the file to delete
 * @return  LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_remove(lv_fs_drv_t *drv, const char *path)
{
#ifdef USE_YAFFS2
    yaffs_unlink(path);

    return errno_to_lvgl_error();

#else
    return LV_FS_RES_NOT_IMP;

#endif
}

/*
 * Truncate the file size to the current position of the read write pointer
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to an 'ufs_file_t' variable. (opened with lv_fs_open)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_trunc(lv_fs_drv_t *drv, void *file_p)
{
#ifdef USE_YAFFS2
    file_t *fp = (file_t *)file_p;
    
    off_t new_size = yaffs_lseek(fp->handle, 0, SEEK_CUR);

    if (new_size > 0)
        yaffs_ftruncate(fp->handle, new_size);

    return errno_to_lvgl_error();

#else
    return LV_FS_RES_NOT_IMP;

#endif
}

/*
 * Rename a file
 * @param drv pointer to a driver where this function belongs
 * @param oldname path to the file
 * @param newname path with the new name
 * @return LV_FS_RES_OK or any error from 'fs_res_t'
 */
static lv_fs_res_t fs_rename(lv_fs_drv_t *drv, const char *oldname, const char *newname)
{
#ifdef USE_YAFFS2
    yaffs_rename(oldname, newname);

    return errno_to_lvgl_error();

#else
    return LV_FS_RES_NOT_IMP;

#endif
}

/*
 * Get the free and total size of a driver in kB
 * @param drv pointer to a driver where this function belongs
 * @param letter the driver letter
 * @param total_p pointer to store the total size [kB]
 * @param free_p pointer to store the free size [kB]
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_free(lv_fs_drv_t *drv, unsigned int *total_p, unsigned int *free_p)
{
#ifdef USE_YAFFS2

    /* YAFFS_MOUNTED_FS_NAME */
    *total_p = ((unsigned int)yaffs_freespace("/ndd"))  >> 10;  // TO kB
    *free_p  = ((unsigned int)yaffs_totalspace("/ndd")) >> 10;

    return errno_to_lvgl_error();

#else
    return LV_FS_RES_NOT_IMP;

#endif
}

/*
 * Initialize a 'fs_read_dir_t' variable for directory reading
 * @param drv pointer to a driver where this function belongs
 * @param rddir_p pointer to a 'fs_read_dir_t' variable
 * @param path path to a directory
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_open(lv_fs_drv_t *drv, void *rddir_p, const char *path)
{
#ifdef USE_YAFFS2
    dir_t *pdir = (dir_t *)rddir_p;

    pdir->handle = (unsigned int)(yaffs_opendir(path));

    return errno_to_lvgl_error();

#else
    return LV_FS_RES_NOT_IMP;

#endif
}

/*
 * Read the next filename form a directory.
 * The name of the directories will begin with '/'
 * @param drv pointer to a driver where this function belongs
 * @param rddir_p pointer to an initialized 'fs_read_dir_t' variable
 * @param fn pointer to a buffer to store the filename
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_read(lv_fs_drv_t *drv, void *rddir_p, char *fn)
{
#ifdef USE_YAFFS2
    dir_t *pdir = (dir_t *)rddir_p;
    yaffs_DIR *dir = (yaffs_DIR *)pdir->handle;
    struct yaffs_dirent *dirent;
    
    dirent = yaffs_readdir(dir);
    *fn = dirent->d_name;

    return errno_to_lvgl_error();

#else
    return LV_FS_RES_NOT_IMP;

#endif
}

/*
 * Close the directory reading
 * @param drv pointer to a driver where this function belongs
 * @param rddir_p pointer to an initialized 'fs_read_dir_t' variable
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_close(lv_fs_drv_t *drv, void *rddir_p)
{
#ifdef USE_YAFFS2
    dir_t *pdir = (dir_t *)rddir_p;
    yaffs_DIR *dir = (yaffs_DIR *)pdir->handle;
    
    yaffs_closedir(dir);
    pdir->handle = 0;

    return errno_to_lvgl_error();

#else
    return LV_FS_RES_NOT_IMP;

#endif
}

#else /* Enable this file at the top */

/* This dummy typedef exists purely to silence -Wpedantic. */
typedef int keep_pedantic_happy;

#endif // #ifdef USE_LVGL

