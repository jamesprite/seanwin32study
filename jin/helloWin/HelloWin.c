#include <windows.h>

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;    // 視窗訊息處理函式的prototype
#pragma comment( lib, "Winmm.lib" ) // for PlaySound function


// Windows 程式第一個進入點 WinMain(...)  

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
// 參數解說:
//    hInstance => 目前 process hInstance 的 Handle
//    hPrevInstance => 永遠是 NULL
//    szCmdLine => 命令列
//    iCmdShow => 指定目前視窗如何被顯示 (SW_HIDE, SW_MAXIMIZE, SW_MINIMIZE, SW_RESTORE)
{
     static TCHAR szAppName[] = TEXT ("HelloWin") ;
     HWND         hwnd ;
     MSG          msg ;

     // ===================== Step 1: 建立並且註冊 Window Class =========================
     WNDCLASS     wndclass ;
     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;                  // << -- 設定視窗訊息處理函式位置
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;                // << -- 指定目前的 class name

     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("This program requires Windows NT!"), 
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }
    
     // ===================== Step 2: 建立視窗並且傳回 window handle ===================== 
     hwnd = CreateWindow (szAppName,                  // window class name
                          TEXT ("The Hello Program"), // window caption
                          WS_OVERLAPPEDWINDOW,        // window style
                          CW_USEDEFAULT,              // initial x position
                          CW_USEDEFAULT,              // initial y position
                          CW_USEDEFAULT,              // initial x size
                          CW_USEDEFAULT,              // initial y size
                          NULL,                       // parent window handle
                          NULL,                       // window menu handle
                          hInstance,                  // program instance handle
                          NULL) ;                     // creation parameters
     
     // ===================== Step 3: 把視窗顯示出來 =====================================
     ShowWindow (hwnd, iCmdShow) ; // 設定顯示方式  SW_SHOW, SW_MAXIMIZE, SW_MINIMIZE 等
     UpdateWindow (hwnd) ; // 送出 WM_PAINT message 到目前的 thread message queue
     
     // ===================== Step 4: Message - Loop 的部分 ==============================
     BOOL bRet;
     UINT wMsgFilterMin=0; // 指定最小可接收的 message 號碼
     UINT wMsgFilterMax=0; // 指定最大可接受的 message 號碼 
                           // (當兩個同為 0 時,表示 GetMessage 會傳回所有的 message)
                           // WM_QUIT 不受 Filter 的限制 -> 以防止不能關視窗的問題出現
     while( (bRet = GetMessage( &msg, NULL, wMsgFilterMin, wMsgFilterMax )) != 0)
     { 
         if (bRet == -1)
         {
             // handle the error and possibly exit
         }
         else
         {
             TranslateMessage(&msg); 
             DispatchMessage(&msg); 
         }
     }

     // 根據 MSDN 建議: GetMessage 發生錯誤時會傳回 -1
     //                 所以原本的程式改為上面的寫法
     // 參考資料: ms-help://MS.MSDNQTR.2004JAN.1033/winui/winui/windowsuserinterface/windowing/messagesandmessagequeues/messagesandmessagequeuesreference/messagesandmessagequeuesfunctions/getmessage.htm
     /*
     while (GetMessage (&msg, NULL, 0, 0))
     {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
     }
     */
     return msg.wParam ;
}

// 視窗訊息處理函式
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     HDC         hdc ;
     PAINTSTRUCT ps ;
     RECT        rect ;
     
     switch (message)
     {
     case WM_CREATE:    // << -- 當視窗被建立時, 事件處理程式碼
          PlaySound (TEXT ("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC) ;
          return 0 ;
          
     case WM_PAINT:     // << -- 當視窗被要求重新繪製時, 事件處理程式碼
          hdc = BeginPaint (hwnd, &ps) ;  // 為目前的視窗準備繪圖的動作,並傳回繪圖用的 device context
          GetClientRect (hwnd, &rect) ;
          DrawText (hdc, TEXT ("Hello, Windows!"), -1, &rect,
                    DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;  // 指定繪製格式  (單行,正中央)         
          EndPaint (hwnd, &ps) ;
          return 0 ;
          
     case WM_DESTROY:
          PostQuitMessage (0) ; // 送出 WM_QUIT 給 thread message queue
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}