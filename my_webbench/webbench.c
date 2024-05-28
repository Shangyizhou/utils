/*
* (C) Radim Kolar 1997-2004
* This is free software, see GNU Public License version 2 for
* details.
*
* Simple forking WWW Server benchmark:
*
* Usage:
*   webbench --help
*
* Return codes:
*    0 - sucess
*    1 - benchmark failed (server is not on-line)
*    2 - bad param
*    3 - internal error, fork failed
* 
*/ 

#include "socket.c"
#include <unistd.h>
#include <sys/param.h>
#include <rpc/types.h>
#include <getopt.h>
#include <strings.h>
#include <time.h>
#include <signal.h>

/* values */
volatile int timerexpired=0;
int speed=0;
int failed=0;
int bytes=0;

/* globals */
int http10=1; /* 0 - http/0.9, 1 - http/1.0, 2 - http/1.1 */
/* Allow: GET, HEAD, OPTIONS, TRACE */
#define METHOD_GET 0
#define METHOD_HEAD 1
#define METHOD_OPTIONS 2
#define METHOD_TRACE 3
#define PROGRAM_VERSION "1.5"
int method=METHOD_GET;
int clients=1;
int force=0;
int force_reload=0;
int proxyport=80;
char *proxyhost=NULL;
int benchtime=30;

/* internal */
int mypipe[2];
char host[MAXHOSTNAMELEN];
#define REQUEST_SIZE 2048
char request[REQUEST_SIZE];

// 长选项值
static const struct option long_options[]=
{
    {"force",no_argument,&force,1},
    {"reload",no_argument,&force_reload,1},
    {"time",required_argument,NULL,'t'},
    {"help",no_argument,NULL,'?'},
    {"http09",no_argument,NULL,'9'},
    {"http10",no_argument,NULL,'1'},
    {"http11",no_argument,NULL,'2'},
    {"get",no_argument,&method,METHOD_GET},
    {"head",no_argument,&method,METHOD_HEAD},
    {"options",no_argument,&method,METHOD_OPTIONS},
    {"trace",no_argument,&method,METHOD_TRACE},
    {"version",no_argument,NULL,'V'},
    {"proxy",required_argument,NULL,'p'},
    {"clients",required_argument,NULL,'c'},
    {NULL,0,NULL,0}
};

/* prototypes */
static void benchcore(const char* host,const int port, const char *request);
static int bench(void);
static void build_request(const char *url);

static void alarm_handler(int signal)
{
    timerexpired=1;
}	

static void usage(void)
{
    fprintf(stderr,
            "webbench [option]... URL\n"
            "  -f|--force               Don't wait for reply from server.\n"
            "  -r|--reload              Send reload request - Pragma: no-cache.\n"
            "  -t|--time <sec>          Run benchmark for <sec> seconds. Default 30.\n"
            "  -p|--proxy <server:port> Use proxy server for request.\n"
            "  -c|--clients <n>         Run <n> HTTP clients at once. Default one.\n"
            "  -9|--http09              Use HTTP/0.9 style requests.\n"
            "  -1|--http10              Use HTTP/1.0 protocol.\n"
            "  -2|--http11              Use HTTP/1.1 protocol.\n"
            "  --get                    Use GET request method.\n"
            "  --head                   Use HEAD request method.\n"
            "  --options                Use OPTIONS request method.\n" 
            "  --trace                  Use TRACE request method.\n"
            "  -?|-h|--help             This information.\n"
            "  -V|--version             Display program version.\n"
           );
}

int main(int argc, char *argv[])
{
    int opt=0;
    int options_index=0;
    char *tmp=NULL;

    if(argc==1)
    {
        usage();
        return 2;
    } 

    // 金山
    while((opt=getopt_long(argc,argv,"912Vfrt:p:c:?h",long_options,&options_index))!=EOF )
    {
        switch(opt)
        {
            case  0 : break;
            case 'f': force=1;break; 
            case 'r': force_reload=1;break;  
            case '9': http10=0;break; 
            case '1': http10=1;break; 
            case '2': http10=2;break; 
            case 'V': printf(PROGRAM_VERSION"\n");exit(0); 
            case 't': benchtime=atoi(optarg);break;	      
            case 'p':  // -p 参数
            /* proxy server parsing server:port */
            tmp=strrchr(optarg,':');
            proxyhost=optarg;
            if(tmp==NULL)
            {
                break;
            }
            if(tmp==optarg)
            {
                fprintf(stderr,"Error in option --proxy %s: Missing hostname.\n",optarg);
                return 2;
            }
            if(tmp==optarg+strlen(optarg)-1)
            {
                fprintf(stderr,"Error in option --proxy %s Port number is missing.\n",optarg);
                return 2;
            }
            *tmp='\0';
            proxyport=atoi(tmp+1);break;
            case ':':
            case 'h':
            case '?': usage();return 2;break;
            case 'c': clients=atoi(optarg);break; 
        }
    }

    // optind 默认为 1
    // 每调用一次 getopt_long，就自增
    // 我们没有对 URL 的参数设置，所以理应 optind = argc - 1
    if(optind==argc) {
        fprintf(stderr,"webbench: Missing URL!\n");
        usage();
        return 2;
    }

    // 默认启动一个客户端，且持续 30s
    if(clients==0) clients=1;
    if(benchtime==0) benchtime=30;
 
    /* Copyright */
    fprintf(stderr,"Webbench - Simple Web Benchmark "PROGRAM_VERSION"\n"
            "Copyright (c) Radim Kolar 1997-2004, GPL Open Source Software.\n"
            );
    
    // 这里传入的就是 url
    build_request(argv[optind]);
 
    // print request info ,do it in function build_request
    /*printf("Benchmarking: ");
 
    switch(method)
    {
        case METHOD_GET:
        default:
        printf("GET");break;
        case METHOD_OPTIONS:
        printf("OPTIONS");break;
        case METHOD_HEAD:
        printf("HEAD");break;
        case METHOD_TRACE:
        printf("TRACE");break;
    }
    
    printf(" %s",argv[optind]);
    
    switch(http10)
    {
        case 0: printf(" (using HTTP/0.9)");break;
        case 2: printf(" (using HTTP/1.1)");break;
    }
 
    printf("\n");
    */

    printf("Runing info: ");

    if(clients==1) 
        printf("1 client");
    else
        printf("%d clients",clients);

    printf(", running %d sec", benchtime);
    
    if(force) printf(", early socket close");
    if(proxyhost!=NULL) printf(", via proxy server %s:%d",proxyhost,proxyport);
    if(force_reload) printf(", forcing reload");
    
    printf(".\n");
    
    return bench();
}

void build_request(const char *url)
{
    char tmp[10];
    int i;

    //bzero(host,MAXHOSTNAMELEN);
    //bzero(request,REQUEST_SIZE);
    memset(host,0,MAXHOSTNAMELEN);
    memset(request,0,REQUEST_SIZE);

    if(force_reload && proxyhost!=NULL && http10<1) http10=1;
    if(method==METHOD_HEAD && http10<1) http10=1;
    if(method==METHOD_OPTIONS && http10<2) http10=2;
    if(method==METHOD_TRACE && http10<2) http10=2;

    // 判断方法，向 request 数组写入
    switch(method)
    {
        default:
        case METHOD_GET: strcpy(request,"GET");break;
        case METHOD_HEAD: strcpy(request,"HEAD");break;
        case METHOD_OPTIONS: strcpy(request,"OPTIONS");break;
        case METHOD_TRACE: strcpy(request,"TRACE");break;
    }

    // 加入空格
    // GET 
    strcat(request," ");

    // http://www.baidu.com/
    // 找到 url 中第一次出现 :// 的字符串
    // 返回 char* 
    if(NULL==strstr(url,"://"))
    {
        // 无 ://，不合法
        fprintf(stderr, "\n%s: is not a valid URL.\n",url);
        exit(2);
    }
    if(strlen(url)>1500)
    {
        // url 过长，不合法
        fprintf(stderr,"URL is too long.\n");
        exit(2);
    }
    // "http://" 与 url 前 7 个字符作比较
    if (0!=strncasecmp("http://",url,7)) 
    { 
        // 不是 HTTP 协议，不合法
        fprintf(stderr,"\nOnly HTTP protocol is directly supported, set --proxy for others.\n");
        exit(2);
    }
    
    /* protocol/host delimiter */
    // 查找 "://" 在 url 中第一次出现后的字符串
    // ://www.baidu.com/，所以需要减去 url 得到偏移值，然后 + 3 跳过 "://"
    // url = http://www.baidu.com/
    // url[i] = w
    i=strstr(url,"://")-url+3;

    // www.baidu.com/
    // 如果没有找到 '/' 即以 '/' 为结尾，即为不符合的 url
    if(strchr(url+i,'/')==NULL) {
        fprintf(stderr,"\nInvalid URL syntax - hostname don't ends with '/'.\n");
        exit(2);
    }
    
    if(proxyhost==NULL)
    {
        /* get port from hostname */
        // index 查找字符在字符串中第一次出现的位置
        // 这里的字符串是 url+i 即 www.baidu.com/
        if(index(url+i,':')!=NULL && index(url+i,':')<index(url+i,'/'))
        {
            // 
            strncpy(host,url+i,strchr(url+i,':')-url-i);
            //bzero(tmp,10);
            memset(tmp,0,10);
            strncpy(tmp,index(url+i,':')+1,strchr(url+i,'/')-index(url+i,':')-1);
            /* printf("tmp=%s\n",tmp); */
            proxyport=atoi(tmp);
            if(proxyport==0) proxyport=80;
        } 
        else
        {
            strncpy(host,url+i,strcspn(url+i,"/"));
        }
        // printf("Host=%s\n",host);
        // strcspn 该函数返回 str1 开头连续都不含字符串 str2 中字符的字符数。
        // 相当于在后面写入了 www.badiu.com 
        strcat(request+strlen(request),url+i+strcspn(url+i,"/"));
    } 
    else
    {
        // printf("ProxyHost=%s\nProxyPort=%d\n",proxyhost,proxyport);
        strcat(request,url);
    }

    // 发出报文添加 http 版本号
    if(http10==1)
        strcat(request," HTTP/1.0");
    else if (http10==2)
        strcat(request," HTTP/1.1");
    
    // 请求行写完，换行，HTTP 检测 \r\n 算换行
    strcat(request,"\r\n");
  
    if(http10>0)
        // 添加请求字段，标注 User-Agent
        strcat(request,"User-Agent: WebBench "PROGRAM_VERSION"\r\n");
    // 添加请求字段，Host 地址
    if(proxyhost==NULL && http10>0)
    {
        strcat(request,"Host: ");
        strcat(request,host);
        strcat(request,"\r\n");
    }
 
    if(force_reload && proxyhost!=NULL)
    {
        strcat(request,"Pragma: no-cache\r\n");
    }
    
    // 如果是超过了 HTTP/1.0 默认长连接
    // 这里采用短连接，所以要手动设置成 Connection: close\r\n
    if(http10>1)
        strcat(request,"Connection: close\r\n");
    
    /* add empty line at end */
    // 加入空行
    if(http10>0) strcat(request,"\r\n"); 
    
    printf("\nRequest:\n%s\n",request);
}

/* vraci system rc error kod */
static int bench(void)
{
    int i,j,k;	
    pid_t pid=0;
    FILE *f;

    /* check avaibility of target server */
    i=Socket(proxyhost==NULL?host:proxyhost,proxyport);
    if(i<0) { 
        fprintf(stderr,"\nConnect to server failed. Aborting benchmark.\n");
        return 1;
    }
    close(i);
    
    /* create pipe */
    if(pipe(mypipe))
    {
        perror("pipe failed.");
        return 3;
    }

    /* not needed, since we have alarm() in childrens */
    /* wait 4 next system clock tick */
    /*
    cas=time(NULL);
    while(time(NULL)==cas)
    sched_yield();
    */

    /* fork childs */
    for(i=0;i<clients;i++)
    {
        /**
         * 父进程返回子进程 pid
         * 子进程返回 0
        */
        pid=fork();
        // 优先创建子进程
        if(pid <= (pid_t) 0)
        {
            /* child process or error*/
            sleep(1); /* make childs faster */
            break;
        }
    }

    if( pid < (pid_t) 0)
    {
        fprintf(stderr,"problems forking worker no. %d\n",i);
        perror("fork failed.");
        return 3;
    }

    // 子进程
    if(pid == (pid_t) 0)
    {
        /* I am a child */
        if(proxyhost==NULL)
            // 传入访问 url 端口号 请求报文
            benchcore(host,proxyport,request);
        else
            benchcore(proxyhost,proxyport,request);

        /* write results to pipe */
        f=fdopen(mypipe[1],"w");
        if(f==NULL)
        {
            perror("open pipe for writing failed.");
            return 3;
        }
        /* fprintf(stderr,"Child - %d %d\n",speed,failed); */
        fprintf(f,"%d %d %d\n",speed,failed,bytes);
        fclose(f);

        return 0;
    }
    // 父进程 
    else
    {
        f=fdopen(mypipe[0],"r");
        if(f==NULL) 
        {
            perror("open pipe for reading failed.");
            return 3;
        }
        
        setvbuf(f,NULL,_IONBF,0);
        
        speed=0;
        failed=0;
        bytes=0;
    
        while(1)
        {
            pid=fscanf(f,"%d %d %d",&i,&j,&k);
            if(pid<2)
            {
                fprintf(stderr,"Some of our childrens died.\n");
                break;
            }
            
            speed+=i;
            failed+=j;
            bytes+=k;
        
            /* fprintf(stderr,"*Knock* %d %d read=%d\n",speed,failed,pid); */
            if(--clients==0) break;
        }
    
        fclose(f);

        /**
         * 每分钟尝试连接总次数
         * 每秒收到字节数
         * 成功次数
         * 失败次数
        */
        printf("\nSpeed=%d pages/min, %d bytes/sec.\nRequests: %d susceed, %d failed.\n",
            (int)((speed+failed)/(benchtime/60.0f)),
            (int)(bytes/(float)benchtime),
            speed,
            failed);
    }
    
    return i;
}

/**
 * 核心函数
 * 通过传入的目标地址端口号，以及请求报文，建立客户端 socket 并发送请求报文
 * 统计发送成功/失败次数，计算接收到的字节数
 * 注册 alarm 信号处理函数实现定时功能，到期发送 alarm 信号设置布尔变量为 true，从而退出创建新连接的循环
 * 发送缓冲区设置的很小，因为请求报文固定，且很小
*/
void benchcore(const char *host,const int port,const char *req)
{
    int rlen;       // 读取长度
    char buf[1500]; // socket 用户缓冲区
    int s,i;        // 记录
    struct sigaction sa;

    /* setup alarm signal handler */
    // 注册 alarm 回调函数
    // 接收信号设置 timerexpired=1;
    sa.sa_handler=alarm_handler;
    sa.sa_flags=0;
    if(sigaction(SIGALRM,&sa,NULL))
        exit(3);
    
    alarm(benchtime); // after benchtime,then exit

    // 记录了读取长度应为请求报文长度
    rlen=strlen(req);
    nexttry:while(1)
    {
        if(timerexpired)
        {
            if(failed>0)
            {
                /* fprintf(stderr,"Correcting failed by signal\n"); */
                failed--;
            }
            return;
        }
        
        // 传入了 url 和 port，在 Socket 函数中发起了连接
        s=Socket(host,port);                          
        if(s<0) { failed++;continue;} 
        // 向 socket 写入 request 报文
        // 返回的写入长度不是 request 长度，则为失败并记录失败次数
        // 因为是简单的报文，所以不考虑超出缓冲区的情况
        if(rlen!=write(s,req,rlen)) {failed++;close(s);continue;}
        if(http10==0) 
        if(shutdown(s,1)) { failed++;close(s);continue;}
        if(force==0) 
        {
            /* read all available data from socket */
            while(1)
            {
                if(timerexpired) break; 
                // 读取响应报文
                i=read(s,buf,1500);
                /* fprintf(stderr,"%d\n",i); */
                if(i<0) 
                { 
                    failed++;
                    close(s);
                    goto nexttry;
                }
                else
                if(i==0) break;
                else
                bytes+=i; // 记录读取字节
            }
        }
        if(close(s)) {failed++;continue;}
        speed++;
    }
}