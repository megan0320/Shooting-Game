#include <windows.h>
#include <gdiplus.h>
#include <mmsystem.h>
#include <process.h>
#include <conio.h>
#include <wchar.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include<winsock.h>
#include "resource.h"
const int n=30,N=3,s=5;
using namespace Gdiplus;
char* getLocapIpAddress() ;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void shooting( void *hwnd ) ;
void udpReceiver(void *hwnd) ;
HINSTANCE hInstance ;
/*  Make the class name into a global variable  */
char szClassName[ ] = "WindowsApp";
int direction = 0 ;
int shootStart0=0;
int shootIng1=0,shootIng2=0,shootIng3=0;
int gameStart=0;
int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    GdiplusStartupInput	 gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup (&gdiplusToken, &gdiplusStartupInput, NULL);

    /* The Window structure */
    hInstance = hThisInstance;
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0; 

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Windows App",       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           1000,                 /* The programs width */
           700,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nFunsterStil);
    //SetTimer(hwnd, 1, 1000, NULL);
   
    _beginthread( shooting, 0, &hwnd );
    _beginthread( udpReceiver, 0, &hwnd );
    
    mciSendString("open ..\\..\\255318__shadydave__race.mp3 type mpegvideo alias bkMusic ", NULL, 0, 0); 
    mciSendString("play bkMusic repeat", NULL, 0, 0);
    mciSendString("setaudio bkMusic volume to 30", NULL, 255, 0);
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }
    GdiplusShutdown(gdiplusToken);
    mciSendString("close bkMusic", NULL, 0, 0);
    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        char str[32] ;
        case WM_KEYDOWN:{
            switch (wParam) {
                case VK_LEFT:
                    direction = -1 ;                    
                    break;
                case VK_RIGHT:
                    direction = 1 ;
                    break;
                case VK_UP:
                    //shootStart0 = 1 ;
                    break;
                case VK_DOWN:
                    direction = 0 ;
                    break;                
                case 'Q':
                    PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
                    break ;
                case ' ':
                    shootStart0 = 1 ;
                    break;    
                case 'S':
                    gameStart=1;
                    break;    
                case 'X':
                    //token = 5 ;
                    //sprintf(str, "X %d",token) ;
                    //MessageBox(hwnd, str , "Key Pressed", MB_OK);
                    //MessageBox(hwnd, getLocapIpAddress() , "Local IP ADDRESS", MB_OK);
                    break;
            }
            
            break;
        }
        
        case WM_PAINT:
        {
            PAINTSTRUCT ps;  //??????
		    //???handle??????handle
            HDC hdc = BeginPaint(hwnd, &ps); 
            Graphics g(hdc); //??????(??)
            
            EndPaint(hwnd, &ps);
        }
            break ;
        
        
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

void shooting( void *hwnd ){
    HWND *myHwnd = (HWND *)hwnd ;
    HDC hdc = GetDC(*myHwnd );	
    	
    RECT rcClient;
    int dx = 10 ;
	int x0=0, y0=0, width=800,height=600 ;
	int picWidth, picHeight ;
    int flyerX[N],flyerY[N];
    int bompX[s],bompY[s];
	int shooterX, shooterY ;
	int imgBullet1X, imgBullet1Y ;
	int midBullet1X,midBullet1Y;
	int midflyerX[N],midflyerY[N];
	
	int midBulletX[n],midBulletY[n];
	
    GetClientRect(*myHwnd, &rcClient);
    width = rcClient.right ;
    height = rcClient.bottom ;
    
    //double buffering starts here	
    HDC hdcMem = CreateCompatibleDC(hdc);
    const int nMemDC = SaveDC(hdcMem);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
    SelectObject(hdcMem, hBitmap);
    Graphics g(hdcMem);  	
    //double buffering ends here
    
	//Graphics g(hdc);
	srand(time(NULL)) ;
	
	Pen pen(Color(255, 0, 50, 50)); 	
	SolidBrush darkRedBrush(Color(255,128,255,128)) ;
	Image		  imgBak (L"..\\..\\blue-sky2.jpg");//sea01b.gif
    TextureBrush  tBrush (&imgBak);
    SolidBrush bulletBrush(Color::Red) ;
	Image flyer(L"..\\..\\single-engine-fighter-animation.gif") ;
	Image imgShooter(L"..\\..\\tank.gif") ;
	Image imgBullet(L"..\\..\\small_red_ball.png") ;
    Image bomp(L"..\\..\\explosion3.gif") ;
	picWidth = flyer.GetWidth() ;
	picHeight = flyer.GetHeight() ;
    
	shooterX = width/2;
	shooterY = height-100 ;
    int shootStart[n];
    int shootIng[n];
    int imgBulletX[n],imgBulletY[n];
    int restart[n];
    int bulletWidth=10,midbulletWidth=5;
    int imgFrameCount = flyer.GetFrameCount(&FrameDimensionTime);	
    int imgFrameCount2 = bomp.GetFrameCount(&FrameDimensionTime);	
    int idx=0,idx2[s];
    int startExplode[s];
    int startExplode0;
    int bomph=bomp.GetHeight()*0.5;
    int shootDecide;
    char str[30];
    int score;
    float time=30;
    
    for(int i=0;i<s;i++){
        
        startExplode[i]=0;
    }
    for(int i=0;i<n;i++){
        
        shootIng[i]=0;
    }
    
    for(int i=0;i<N;i++){
        flyerX[i] = -(rand() % width/3 );
        flyerY[i] = rand()%height/3 ;
    }
    do { 
        g.FillRectangle(&tBrush,x0,y0,width,height);
        g.DrawRectangle(&pen,x0,y0,width,height);
        
        wchar_t str2[10];
        //swprintf(str, L"press'S' to start the game") ;
        //MessageBox(hwnd, str , "Key Pressed", MB_OK);
        FontFamily  fontFamily(L"Times New Roman");
        Font  font(&fontFamily, 50, FontStyleRegular, UnitPixel);
        PointF pointF(280.0f, 300.0f);
        SolidBrush  solidBrush(Color(255, 255, 255, 255));
        g.DrawString(L"press'S' to start the game", -1, &font, pointF, &solidBrush);
        //double buffering start here
        BitBlt(hdc, x0, y0, width+1, height+1, hdcMem, x0, y0, SRCCOPY);
        //double buffering end here    
    }while(gameStart==0);    
    score=0;
    do{
        flyer.SelectActiveFrame(&FrameDimensionTime, idx++);
        if(idx == imgFrameCount) idx= 0;
        
        g.FillRectangle(&tBrush,x0,y0,width,height);
        g.DrawRectangle(&pen,x0,y0,width,height);
        
        for(int i=0;i<N;i++){
            if(flyerX[i]>=width){
                flyerX[i] = -(rand() % width/3 );
                flyerY[i] = rand()%height/3 ;
            }
            midflyerY[i]=flyerY[i]+flyer.GetHeight()*0.5;
    	    midflyerX[i]=flyerX[i]+flyer.GetWidth()*0.5;
            g.DrawImage(&flyer, flyerX[i], flyerY[i]) ;       
        }        
        g.DrawImage(&imgShooter, shooterX, shooterY) ;
        if(shootStart0!=0){
            for(int i=0;i<n;i++){            
                if(shootIng[i]==0){
                    shootStart0=0;
                    shootDecide=i;
                    PlaySound((LPCSTR) "..\\..\\gunShot1.wav", NULL, SND_FILENAME | SND_ASYNC); 
                   
                    break;
                }
            }
                            
            imgBulletX[shootDecide]=shooterX+imgShooter.GetWidth()*0.5-midbulletWidth;
            imgBulletY[shootDecide]=shooterY;
            shootIng[shootDecide]=1;
            sprintf(str, "%d",shootDecide) ;
            //MessageBox(*myHwnd, str , "Key Pressed", MB_OK);        
                    
            
        } 
        
        for(int i=0;i<n;i++){
            if(shootIng[i]!=0){
                for(int j=0;j<N;j++){
                    
                    midBulletY[i]=imgBulletY[i]+ midbulletWidth;
	                midBulletX[i]=imgBulletX[i]+ midbulletWidth;
                    g.DrawImage(&imgBullet, imgBulletX[i], imgBulletY[i],bulletWidth,bulletWidth) ;
                
                    if(midflyerX[j]>midBulletX[i]){
                        if((midBulletY[i]-midflyerY[j]<flyer.GetHeight()*0.5+imgBullet.GetHeight()*0.5)&&
                            (midflyerX[j]-midBulletX[i]<flyer.GetWidth()*0.5+imgBullet.GetWidth()*0.5)){
                            shootIng[i]=0;
                            flyerX[j] = -(rand() % width/3 );
                            for(int k=0;k<s;k++){       
                                if(startExplode[k]==0){
                                     startExplode0=k;
                                    
                                     break;
                                } 
                            }   
                            score+=100;
                            bompX[startExplode0]=midflyerX[j]-bomph;
                            bompY[startExplode0]=midflyerY[j]-bomph;
                            midBulletX[startExplode0]=-1;
                
                           
                            startExplode[startExplode0]=1;
                            idx2[startExplode0]=0;
                            PlaySound((LPCSTR) "..\\..\\explosion1.wav", NULL, SND_FILENAME | SND_ASYNC);
                        } 
                    }
                    if(midBulletX[i]>midflyerX[j]){
                        if((midBulletY[i]-midflyerY[j]<flyer.GetHeight()*0.5+5)&&
                            (midBulletX[i]-midflyerX[j]<flyer.GetWidth()*0.5+5)){
                            shootIng[i]=0;
                            flyerX[j] = -(rand() % width/3 );
                            for(int k=0;k<s;k++){       
                                if(startExplode[k]==0){
                                     startExplode0=k;
                                    
                                     break;
                                } 
                            }   
                            score+=100;
                            
                            bompX[startExplode0]=midflyerX[j]-bomph;
                            bompY[startExplode0]=midflyerY[j]-bomph;
                            midBulletX[startExplode0]=-1;
                            sprintf(str, "%d %d %d",i,j,startExplode0) ;
                            //MessageBox(*myHwnd, str , "Key Pressed", MB_OK);
                            startExplode[startExplode0]=1;
                            idx2[startExplode0]=0;
                            PlaySound((LPCSTR) "..\\..\\explosion1.wav", NULL, SND_FILENAME | SND_ASYNC);
                            }
                        }
                }  //for(int j=0;j<N;j++)
                 
            } 
            //if(shootIng[i]!=0){  
                          
        }
        
        for(int i=0;i<s;i++){  
            if(startExplode[i]==1){
                bomp.SelectActiveFrame(&FrameDimensionTime, idx2[i]);
                g.DrawImage(&bomp, bompX[i], bompY[i]);
                idx2[i]++;
                if(idx2[i] >= imgFrameCount2) startExplode[i]=0;;
             } 
        }                   
        for(int i=0;i<n;i++){
            imgBulletY[i]-=10;  
            if( imgBulletY[i]<=0){
                 imgBulletY[i]=height;
                 shootIng[i]=0;
            }
        }    
        for(int j=0;j<N;j++){
            flyerX[j]+=dx;
        }     
        shooterX+=(direction*5); 
        time-=(0.02); 
        if(time<=0){
            gameStart=0; 
            wchar_t str3[10];
             swprintf(str3, L"game over   u got score %d ",score) ;
            //MessageBox(hwnd, str , "Key Pressed", MB_OK);
            FontFamily  fontFamily(L"Times New Roman");
            Font  font(&fontFamily, 50, FontStyleRegular, UnitPixel);
            PointF pointF(260.0f, 250.0f);
            SolidBrush  solidBrush(Color(255, 255, 255, 255));
            g.DrawString(str3, -1, &font, pointF, &solidBrush);  
        }    
        if(shooterX<0||shooterX>(width-imgBullet.GetWidth()-bulletWidth))
            direction*=-1;
        wchar_t str[30];
        swprintf(str, L"score %d  time %3.0f",score,time) ;
        //MessageBox(hwnd, str , "Key Pressed", MB_OK);
        FontFamily  fontFamily(L"Times New Roman");
        Font  font(&fontFamily, 24, FontStyleRegular, UnitPixel);
        PointF pointF(30.0f, 10.0f);
        SolidBrush  solidBrush(Color(255, 0, 255, 255));
        g.DrawString(str, -1, &font, pointF, &solidBrush);

           
        //double buffering start here
        BitBlt(hdc, x0, y0, width+1, height+1, hdcMem, x0, y0, SRCCOPY);
        //double buffering end here    
        Sleep(20) ;
    }
    // Repeat while RunMutex is still taken.  drawBouncingPicture
    while ( gameStart==1 );
    //double buffering start here
    RestoreDC(hdcMem, nMemDC);
    DeleteObject(hBitmap);
    //double buffering end here   
    ReleaseDC(*myHwnd, hdc);
}


void udpReceiver(void *hwnd)
{
     //???? ??-????- ??????-???  -lws2_32
     HWND *myHwnd = (HWND *)hwnd ;
     SOCKET socket1;
     WSADATA wsaData;
     int ErrorCode;
     if(WSAStartup(MAKEWORD(2,1),&wsaData))// windows socket dll;
     {
        MessageBox(*myHwnd, "Winsock initiate failed!!" , "UDP receiver", MB_OK);
        //printf("Winsock initiate failed!!\n");
        WSACleanup();
        return ;
     }
     
     //printf("Winsock start...\n");
     struct sockaddr_in local;
     struct sockaddr_in client;
     int len = sizeof(client);
     local.sin_family = AF_INET;
     local.sin_port = htons(1234);  //the port need listened
     local.sin_addr.s_addr = INADDR_ANY; //localhost
     
     socket1 = socket(AF_INET,SOCK_DGRAM,0);
     bind(socket1,(struct sockaddr*)&local,sizeof(local));
     //MessageBox(*myHwnd, getLocapIpAddress() , "Local IP ADDRESS", MB_OK);
     while (1)
     {
        char buffer[1024] = "\0";
        //printf("waiting message from the clients...\n");
        if(recvfrom(socket1,buffer,sizeof(buffer),0,(struct sockaddr*)&client,&len) != SOCKET_ERROR)
        {
            //MessageBox(*myHwnd, buffer , "UDP receiver", MB_OK);
            if( strcmp("A",buffer) == 0) direction = -1 ;                
            if( strcmp("D",buffer) == 0) direction = 1 ; ;   
            if( strcmp("W",buffer) == 0) shootStart0 = 1 ;   
            if( strcmp("S",buffer) == 0) direction = 0 ;               
            //sprintf(buffer,"%d",argb) ;
            //MessageBox(*myHwnd, buffer , "UDP receiver", MB_OK);
            //send sth to the client
            //sendto(socket1,buffer,sizeof(buffer),0,(struct sockaddr*)&client,len);
        }                                              
     }
     closesocket(socket1);
     
     return ;
}
char* getLocapIpAddress(){
    char ac[80];
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
        return NULL;
    }
    if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR)return NULL;    
    struct hostent *phe = gethostbyname(ac);
    if (phe == 0)return NULL;    
    
    struct in_addr addr;
    memcpy(&addr, phe->h_addr_list[0], sizeof(struct in_addr));
    WSACleanup();
    return inet_ntoa(addr);
}
