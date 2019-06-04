#include "cdes/hex.h"
#include "cdes/encrypt.h"
#include "cdes/buffer.h"
#include "tools.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include <getopt.h>

#define BUFFER_SIZE 102400

char global_mode = 'e';
int verbose = 0;
int parallel = 20;
int batchSize = 1000000;

/*******************list*********************/
typedef struct node {
    char * data;
    struct node* next;
} NODE;

typedef struct list {
    NODE* head;
    NODE* tail;
} LIST;

void list_init(LIST* list) {
    list->head = NULL;
    list->tail = NULL;
}

void list_destroy_node(NODE * node){
    free(node->data);
    free(node);
}

void list_append(LIST* list, const char* data){
    list_append_internal(list, data, 1);
}

void list_append_internal(LIST* list, const char* data, int create) {
    NODE* temp = (NODE*) malloc( sizeof(NODE) );
    if (temp == NULL) {
        exit(0); // no memory available
    }
    if(create){
        temp->data = (char *)malloc( strlen(data) + 1 );
        strncpy(temp->data, data, strlen(data));
        *(temp->data + strlen(data) ) = 0;
        //snprintf(temp->data, strlen(data) + 1, "%s", data);
    }else{
        temp->data = data;
    }
    temp->next = NULL;
    if(list->head == NULL && list->tail == NULL){
        list->head = temp;
        list->tail = temp;
    }else{
        list->tail->next = temp;
        list->tail = temp;
    }
}

NODE * list_remove_first(LIST* list){
    if(list->head == NULL && list->tail == NULL){
        return NULL;
    }
    NODE* temp = list->head->next;
    NODE * firstObj = list->head;
    list->head = temp;
    if(temp == NULL){
        list->tail = NULL;
    }
    return firstObj;
}

int list_empty(LIST* list){
    if(list->head == NULL && list->tail == NULL ){
        return 1;
    }else{
        return 0;
    }
}

size_t list_size(LIST* list) {
    size_t count = 0;
    NODE *tmpPtr = list->head;
    while (tmpPtr != NULL) {
        count++;
        tmpPtr = tmpPtr->next;
    }
    return count;
}

void list_print(LIST* list) {
    NODE *tmpPtr = list->head;
    int count = 0;
    while (tmpPtr != NULL) {
        printf("pos = %d, value = %s \n",count,tmpPtr->data);
        tmpPtr = tmpPtr->next;
        count++;
    }
}
/****************************************/

typedef struct thread_context {
    int index;
    LIST input_list;
    LIST output_list;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_attr_t attr;
    pthread_t thd;
    volatile int flag;
} ThreadContext;

void* worker_func(void *arg){
    ThreadContext * cxt = (ThreadContext *)arg;
    int count = 0;
    while(1){
        pthread_mutex_lock(&cxt->mutex);
        if( list_empty(&cxt->input_list) ){
            //wait for data
            if(verbose) printf("-- worker %d wait for more data\n",cxt->index);
            pthread_cond_wait(&cxt->cond,&cxt->mutex);
        }
        if( list_empty(&cxt->input_list) ){
            if(verbose) printf("-- worker %d have no data to handle, set flag=1\n",cxt->index);
            cxt->flag ++;
            pthread_mutex_unlock(&cxt->mutex); 
            continue;  //wait for the following data
        }
        count++;
        NODE* node = list_remove_first(&cxt->input_list);
        if(verbose) printf("-- worker %d handle data %d, {{%s}}\n",cxt->index,count,node->data);
        if(global_mode == 'd'){
            char * plainText = NULL;
            des_decrypt(node->data,&plainText);
            if(plainText){
                list_append_internal(&cxt->output_list, plainText, 0);
            }
        }
        if(global_mode == 'e'){
            char * cypherText = NULL;
            des_encrypt(node->data,strlen(node->data),&cypherText);
            if(cypherText){
                list_append_internal(&cxt->output_list, cypherText, 0);
            }
        }
        list_destroy_node(node);
        pthread_mutex_unlock(&cxt->mutex); 
        //sleep(1);
    }
    
    if(verbose) printf("++ worker %d finish\n",cxt->index);
}


int main(int argc, char** argv){

    //For test
    /*LIST list;
    list_init(&list);
    list_append(&list, "1111");
    list_append(&list, "2222");
    list_append(&list, "3333");
    list_append(&list, "4444");
    list_append(&list, "55555");
    list_print(&list);
    printf("befor remove, size = %d \n", list_size(&list) );
    list_destroy_node(list_remove_first(&list));
    list_destroy_node(list_remove_first(&list));
    printf("after remove, size = %d \n", list_size(&list) );
    list_print(&list);
    if(1 == 1){
        exit(0);
    }*/

	unsigned int iseed = (unsigned int)time(NULL);
	srand (iseed);

    time_t t_start, t_end;
    t_start = time(NULL);

    if(argc < 5){
        printf("Options: [-dv] [-b <batch size>] -p <parallel> -i <input file> -o <output file>\n");
        exit(1);
    }

    char * parallel_str = NULL;
    char * mode = NULL;
    char * input_file = NULL;
    char * output_file = NULL;
    char * batch_size = NULL;

    int option_index = 0;
    while (( option_index = getopt(argc, argv, "p:i:o:b:dv")) != -1){
        switch (option_index) {
            case 'p':
                parallel_str = optarg;
                break;
            case 'i':
                input_file = optarg;
                break;
            case 'o':
                output_file = optarg;
                break;
            case 'b':
                batch_size = optarg;
                break;
            case 'd':
                global_mode = 'd';
                break;
            case 'v':
                verbose = 1;
                break;
            default:
                printf("Options: [-dv] [-b <batch size>] -p <parallel> -i <input file> -o <output file>\n");
                exit(1);
        }
    }

    if(batch_size){
        batchSize = atoi(batch_size);
    }
    if(parallel_str){
        parallel = atoi(parallel_str);
    }
    
    ThreadContext * context = (ThreadContext *)malloc( parallel * sizeof(ThreadContext) );
    ThreadContext * contextPtr = NULL;
    for(int i=0; i<parallel; ++i){
        contextPtr = context + i;
        list_init(&contextPtr->input_list);
        list_init(&contextPtr->output_list);
        pthread_mutex_init(&contextPtr->mutex,NULL); 
        pthread_cond_init(&contextPtr->cond,NULL);
        contextPtr->index = i;
        contextPtr->flag = 0;
        pthread_attr_init(&contextPtr->attr);
        pthread_attr_setdetachstate (&contextPtr->attr, PTHREAD_CREATE_DETACHED);
        if(pthread_create(&contextPtr->thd, &contextPtr->attr, worker_func, contextPtr)){
            exit(0);
        }
    }

    FILE *input_fp; 
    if(verbose) printf("input file : [%s]\n",input_file);
    if((input_fp = fopen(input_file,"r")) == NULL){
        perror("open input file failed");
        exit(1);
    }
    
    FILE *output_fp; 
    if(verbose) printf("output file : [%s]\n",output_file);
    if((output_fp = fopen(output_file,"w")) == NULL){
        perror("open output file failed");
        exit(1);
    }

    char StrLine[BUFFER_SIZE] = {0};
    int batchIndex = 0;

BATCH_LOOP:
    batchIndex++;
    if(verbose) printf("--------------begin to handle batch data %d--------------\n", batchIndex);
    int count = 0;
    buffer * longLineBuf = NULL;
    while (!feof(input_fp)) 
    {
        if( count > batchSize ){
            break;
        }
        if( !fgets(StrLine,BUFFER_SIZE,input_fp) ){
            break;
        }
        int i = strlen(StrLine); 
        //printf("len=%d\n",i);
        if(StrLine[i-1] == '\n'){
            StrLine[i-1] = 0;
        }else{
            //printf("too long , exit\n");
            //exit(0);
            //这行太长了，10k还没有读完一行，只能上buffer了
            longLineBuf = buffer_init();
            buffer_append_string(longLineBuf, StrLine);
            do{
                fgets(StrLine,BUFFER_SIZE,input_fp);
                i = strlen(StrLine); 
                if(StrLine[i-1] == '\n'){
                    StrLine[i-1] = 0;
                    buffer_append_string(longLineBuf, StrLine);
                    break;
                }else{
                    buffer_append_string(longLineBuf, StrLine);
                }
            }while(1);
        }
        int index = count % parallel;
        count++;
        if(verbose) printf(">> take data to worker %d , %d, %d\n",index,count,i);
        contextPtr = context + index;

        //放入一个线程的input队列
        pthread_mutex_lock(&contextPtr->mutex);
        if(longLineBuf != NULL){
            list_append(&contextPtr->input_list, longLineBuf->ptr);
            buffer_free(longLineBuf);
            longLineBuf = NULL;
        }else{
            list_append(&contextPtr->input_list, StrLine);
        }
        pthread_cond_signal(&contextPtr->cond);
        pthread_mutex_unlock(&contextPtr->mutex);
    } 
    if(count == 0){
        goto COMPLETE;
    }

    //通知每个工作线程，数据发送完毕(可能这种做法是没用的)
    for(int i=0; i<parallel; ++i){
        contextPtr = context + i;
        pthread_mutex_lock(&contextPtr->mutex);
        pthread_cond_signal(&contextPtr->cond);
        pthread_mutex_unlock(&contextPtr->mutex);
    }

    //检查各个线程是否已经工作完毕
    if(verbose) printf("begin to check if or not finish\n");
    int finish = 0;
    int * indexArray = (int *)malloc( sizeof(int) * parallel );
    int ind;
    for(ind=0; ind < parallel; ind++) {
        indexArray[ind] = 0;
    }
    while(1){
        for(int i=0; i<parallel; ++i){
            contextPtr = context + i;
            if( indexArray[i] == 1 ){
                continue;
            }
            pthread_mutex_lock(&contextPtr->mutex);
            if(contextPtr->flag == 0){
                //工作线程还没有处理完
                pthread_cond_signal(&contextPtr->cond);
            }else{
                //工作线程处理完了
                finish++;
                contextPtr->flag = 0;  //把标志改回未处理完
                indexArray[i] = 1;   //下次不再检查这个线程的标记了
            }
            pthread_mutex_unlock(&contextPtr->mutex);
        }
        if(finish == parallel){
            if(verbose) printf("!!!handle finish\n");
            break;
        }else{
            if(verbose) printf("!!handle not finish, parallel = %d, finish = %d\n",parallel,finish);
            sleep(1);
        }
    }
    free(indexArray);

    //遍历每个工作线程的输出队列，输出到文件
    if(verbose) printf("begin to output\n");
    while(1){
        int finished = 0;
        for(int i=0; i<parallel; ++i){
            contextPtr = context + i;
            if( list_empty(&contextPtr->output_list) ){
                //没有数据了
                finished = 1;
                break;
            }else{
                NODE* node = list_remove_first(&contextPtr->output_list);
                fprintf(output_fp,"%s\n",node->data);
                list_destroy_node(node);
            }
        }
        if(finished){
            break;
        }
    }
    fflush(output_fp);
    goto BATCH_LOOP; //继续处理下一个批次的数据

COMPLETE:
    //关闭输入文件
    fclose(input_fp);

    //关闭输出文件
    fclose(output_fp);
    if(verbose) printf("output finish\n");

    t_end = time(NULL);
    printf("cost time: %.0f s\n", difftime(t_end,t_start)) ;

    //这里没有释放堆里的内存，因为这个命令执行完，进程退出就自动释放了

    return 0;
}


