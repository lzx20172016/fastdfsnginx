#include"upload_file.h"
#include"../util.h"
int save_one_file(char*filename)
{

    char file_id[FILE_ID_LEN]={0};
    redisContext *conn=NULL;
    time_t now;
    char time_str[TIME_STR_MAX]={0};
    int ret=0;

    //将文件存入fastdf中-->fileid
    ret=fdfs_upload_by_filename(filename,file_id);
    if(ret==-1)
    {
            printf("fdfs upload%s error\n",filename);
            goto END;
    }

    //将fieild-->FILE——INFO——LIST
    conn=rop_connectdb_nopwd("127.0.0.1","6379");
    if(conn==NULL)
    {
        return -1;
        goto END;
    }
    ret=rop_list_push(conn,FILE_USER_LIST,file_id);
    if(ret!=0)
    {
        printf("push %s to %s error\n",file_id,FILE_USER_LIST);
        goto END;
    }
    //存入文件属性hash中
    //文件名
    ret=rop_hash_set(conn,FILEID_NAME_HASH,file_id,filename);
    if(ret!=0)
    {
        printf("hset %s to %s error\n",file_id,FILEID_NAME_HASH);
        goto END;
    }

    now=time(NULL);//获得当前系统时间
    strftime(time_str,TIME_STR_MAX,"%Y-%m-%d %H:%M:%S",localtime(&now));
//创建时间
    ret=rop_hash_set(conn,FILEID_TIME_HASH,file_id,time_str);
    if(ret!=0)
    {
        printf("hset %s to %s error\n",file_id,FILEID_TIME_HASH);
        goto END;
    }
//文件所属用户名
    ret=rop_hash_set(conn,FILEID_USER_HASH,file_id,"ICE");
    if(ret!=0)
    {
        printf("hset %s to %s error\n",file_id,FILEID_USER_HASH);
        goto END;
    }

//点击量
    ret=rop_hash_set(conn,FILEID_PV_HASH,file_id,"0");
    if(ret!=0)
    {
        printf("hset %s to %s error\n",file_id,FILEID_PV_HASH);
        goto END;
    }

    char suffix[16]={0};
    get_file_suffix(filename,suffix);


//文件的类型
    ret=rop_hash_set(conn,FILEID_TYPE_HASH,file_id,"2");
    if(ret!=0)
    {
        printf("hset %s to %s error\n",file_id,FILEID_TYPE_HASH);
        goto END;
    }

    char file_url[FILE_URL_LEN]={0};
    //拼接一个URL
    strcat(file_url,"http://");
    strcat(file_url,STORAGE_IP);
    strcat(file_url,"/");
    strcat(file_url,file_id);
//文件URL
    ret=rop_hash_set(conn,FILEID_URL_HASH,file_id,file_url);
    if(ret!=0)
    {
        printf("hset %s to %s error\n",file_id,FILEID_PV_HASH);
        goto END;
    }
//文件的分享状态
    ret=rop_hash_set(conn,FILEID_SHARED_STATUS_HASH,file_id,"0");
    if(ret!=0)
    {
        printf("hset %s to %s error\n",file_id,FILEID_SHARED_STATUS_HASH);
        goto END;
    }
    rop_disconnect(conn);
END:
    return 0;

}
