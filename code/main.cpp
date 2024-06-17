


#include <windows.h>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>


//stringstream
#include <iomanip>
using namespace std;



#define Debug
const string pluginVersion = "1.0.0";
FILE* ConsoleWindow;

typedef void* HGLOBAL;
char* resBuf;

char* pluginDirectory;




//{{{
int main(int argc, char* argv[]) {
    printf( "%s\n" , argv[0] );
    return 0;
}
//}}}
//{{{
extern "C" __declspec(dllexport) bool __cdecl load(HGLOBAL h, long len){
//hにはdllまでのLogFilePathが入っている。
//lenはアドレスの長さ。\0の分は入っていない。

#ifdef Debug
    AllocConsole();
    //標準出力(stdout)を新しいコンソールに向ける
    freopen_s(&ConsoleWindow, "CONOUT$", "w+", stdout);
    //標準エラー出力(stderr)を新しいコンソールに向ける
    freopen_s(&ConsoleWindow, "CONOUT$", "w+", stderr);
    //文字コードをutf-8に変更。
    system( "chcp 65001 > /nul" );
#endif

    pluginDirectory = (char*)malloc(sizeof(char) * (len + 1 ));
    memset( pluginDirectory , '\0' , ( len + 1 ));
    memcpy( pluginDirectory , (char*)h , (int)len  );
    GlobalFree( h );



    char* err = NULL;

    return true;
}
//}}}
//{{{
extern "C" __declspec(dllexport) bool __cdecl unload(void){
    free( pluginDirectory );

#ifdef Debug
    FreeConsole();
#endif

    pluginDirectory = NULL;

    return true;
}
//}}}


extern "C" __declspec(dllexport) HGLOBAL __cdecl request(HGLOBAL h, long *len){
    char req[*len+1];
    memset( req , '\0' , *len+1 );
    memcpy( req , (char*)h , *len );

//#ifdef Debug
//    printf( "%s----\n" , req );
//#endif

    GlobalFree( h );


    bool NOTIFY = false;
    resBuf = NULL;

    char* ID        = NULL;
    char* Sender    = NULL;


    char* Reference0 = NULL;
    char* Reference1 = NULL;
    char* Reference2 = NULL;
    char* Reference3 = NULL;
    char* Reference4 = NULL;
    char* Reference5 = NULL;
    char* Reference6 = NULL;
    char* Reference7 = NULL;

    char  sepLine[]    = "\r\n";
    char  sepLR[] = ": ";
    char* tp ;
    tp = strtok( req , sepLine );
    while( tp != NULL ){
        if ( tp == "GET PLUGIN/2.0" ){
            tp = strtok( NULL , sepLine );
            continue;
        } else if ( tp == "NOTIFY PLUGIN/2.0" ){
            NOTIFY = true;
            tp = strtok( NULL , sepLine );
            continue;
        }

        //左右分割を試みる。
        char* checkR = strstr( tp , sepLR );
        if ( checkR == NULL ){
            tp = strtok( NULL , sepLine );
            continue;
        }


        int Lsize = strlen( tp ) - strlen( checkR ) ;
        char L[ Lsize + 1 ] ;
        memset( L , '\0' , Lsize + 1 );
        memcpy( L , tp , Lsize);

        int Rsize = strlen( tp ) - ( Lsize + strlen( sepLR ) );
        //char R[ Rsize + 1 ] ;
        //memset( R , '\0' , Rsize + 1 );
        //memcpy( R , &tp[ Lsize + strlen( sepLR ) ] , Rsize);

        //printf( "%s\n" , L );
        //printf( "%s\n" , R );
        
        if ( strcmp( L , "ID" ) == 0 ) {
            ID = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( ID , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Sender" ) == 0 ) {
            Sender = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Sender , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Reference0" ) == 0 ) {
            Reference0 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Reference0 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Reference1" ) == 0 ) {
            Reference1 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Reference1 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Reference2" ) == 0 ) {
            Reference2 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Reference2 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Reference3" ) == 0 ) {
            Reference3 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Reference3 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Reference4" ) == 0 ) {
            Reference4 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Reference4 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Reference5" ) == 0 ) {
            Reference5 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Reference5 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Reference6" ) == 0 ) {
            Reference6 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Reference6 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Reference7" ) == 0 ) {
            Reference7 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Reference7 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        //} else if ( strcomp( L , "" ) == 0 ) {
        }
        tp = strtok( NULL , sepLine );
    }



    if ( ID != NULL ) {

        if ( strcmp( ID , "version" ) == 0 ) {
            //char res_buf[] = "PLUGIN/2.0 200 OK\r\nCharset: UTF-8\r\nValue: 1.0.0\r\n\r\n";
            string resStr = "PLUGIN/2.0 200 OK\r\nCharset: UTF-8\r\nValue: " + pluginVersion + "\r\n\r\n";
            int i = strlen( resStr.c_str() );
            char* res_buf;
            res_buf = (char*)calloc( i + 1 , sizeof(char) );
            memcpy( res_buf , resStr.c_str() , i );
            resBuf = res_buf;

        } else if ( strcmp( ID , "OnSecondChange" ) == 0 ) {
        ////プラグインの存在を通知する機能
        } else if ( strcmp( ID , "OnGhostBoot" ) == 0 || strcmp( ID , "OnVersion" ) == 0 ) {
            // string OnExistPluginMailBox = "PLUGIN/2.0 200 OK\r\nCharset: UTF-8\r\nEvent: OnExistPluginMailBox\r\nReference0: " + pluginVersion + "\r\n\r\n";
            // int i = strlen( OnExistPluginMailBox.c_str() );
            // char* res_buf;
            // res_buf = (char*)calloc( i + 1 , sizeof(char) );
            // memcpy( res_buf , OnExistPluginMailBox.c_str() , i );
            // resBuf = res_buf;

        ////Userが触る機能
        //メールボックス
        //┌ └ ┐ ┘ ├ ┤ ─ ┬ ┼ ┴
        //\\_a[OnCheckMail,0,0] ───未読メール─── \\_a
        //\\_a[OnCheckMail,1,0] ───既読メール─── \\_a┼
        //\\_a[OnCheckMail,2,0] ───選択メール─── \\_a┼
        } else if ( strcmp( ID , "OnMenuExec" ) == 0 ) {

            // string resStr = "PLUGIN/2.0 200 OK\r\nCharset: UTF-8\r\nScript: \\_q┌┬─MailBoxSYSTEM v" + pluginVersion + "─┬┐ \\n├┼────────────┼┤ \\n├┼ \\q[───未読メール───,OnCheckMail,0,0] ┼┤ \\n├┼────────────┼┤ \\n├┼ \\q[───既読メール───,OnCheckMail,1,0] ┼┤ \\n├┼────────────┼┤ \\n├┼ \\q[───個別メール───,OnCheckMail,2,0] ┼┤ \\n├┼────────────┼┤ \\n├┼────────────┼┤ \\n├┼ \\q[────閉じる────,] ┼┤ \\n└┴────────────┴┘ \\_q \r\nScriptOption: nobreak,notranslate\r\n\r\n";
            // int i = strlen( resStr.c_str() );
            // char* res_buf;
            // res_buf = (char*)calloc( i + 1 , sizeof(char) );
            // memcpy( res_buf , resStr.c_str() , i );
            // resBuf = res_buf;


        } else if ( strcmp( ID , "OnGhostExit" ) == 0 ) {
#ifdef Debug
            printf( "%s\n" , Reference0 );
            printf( "%s\n" , Reference1 );
            printf( "%s\n" , Reference2 );
            printf( "%s\n" , Reference3 );
            printf( "%s\n" , Reference4 );
#endif

        }
    }

    //返すものがなかった時
    if ( resBuf == NULL ){
        char res_buf[] = "PLUGIN/2.0 204 No Content\r\nCharset: UTF-8\r\n\r\n";
        resBuf = res_buf;
    }
    if ( ID != NULL ){ free( ID ); }
    if ( Sender != NULL ){ free( Sender ); }
    if ( Reference0 != NULL ){ free( Reference0 ); }
    if ( Reference1 != NULL ){ free( Reference1 ); }
    if ( Reference2 != NULL ){ free( Reference2 ); }
    if ( Reference3 != NULL ){ free( Reference3 ); }
    if ( Reference4 != NULL ){ free( Reference4 ); }
    if ( Reference5 != NULL ){ free( Reference5 ); }
    if ( Reference6 != NULL ){ free( Reference6 ); }
    if ( Reference7 != NULL ){ free( Reference7 ); }
    
    //pluginは2.0で返す。
    //char res_buf[] = "PLUGIN/2.0 204 No Content";
    *len = strlen(resBuf);
    HGLOBAL ret = GlobalAlloc(GPTR, (SIZE_T)(*len));
    memcpy(ret, (void*)(resBuf), *len);
    return ret;
}




