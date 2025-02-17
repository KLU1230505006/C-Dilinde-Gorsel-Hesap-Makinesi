    #include <windows.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <tchar.h>/* bu k�t�phane  Microsoft Visual Studio'nun �oklu karakter seti (MBCS) ve geni� karakter seti (Unicode) gibi farkl� karakter 
    setleri aras�nda ge�i� yapmaya yard�mc� olan bir dizi makro tan�mlar. Bu, yaz�l�m geli�tiricilere,
    kodlar�n� farkl� karakter setlerinde derlemek i�in daha fazla esneklik sa�lar. */

    LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);//Bu sat�r, Windows uygulamalar�nda bir pencere i�leme fonksiyonunu  tan�mlar. 
    /*
    HWND hWnd: ��lemin ba�l� oldu�u pencerenin tan�mlay�c�s� (handle).
    UINT message: ��lenen olay�n t�r�n� belirten bir say�sal de�er.
    WPARAM wParam: ��leme ge�irilen ek bilgiler, genellikle olaya �zg� veri.
    LPARAM lParam: ��leme ge�irilen ek bilgiler, genellikle olaya �zg� veri.
    Fonksiyonun d�n�� de�eri LRESULT t�r�ndedir ve genellikle olaya �zg� bir de�er i�erir. */

    void removeTrailingZeros(char* str);//bu fonksiyon i�lemin sonucundaki gereksiz s�f�rlar� silmemize yard�mc� olur.

    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
        PSTR szCmdLine, int iCmdShow) {
        static TCHAR szAppName[] = TEXT("Calculator");
        HWND hwnd;
        MSG msg;
        WNDCLASS wndclass;

        wndclass.style = CS_HREDRAW | CS_VREDRAW;
        wndclass.lpfnWndProc = WndProc;
        wndclass.cbClsExtra = 0;
        wndclass.cbWndExtra = 0;
        wndclass.hInstance = hInstance;
        wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wndclass.lpszMenuName = NULL;
        wndclass.lpszClassName = szAppName;

        if (!RegisterClass(&wndclass)) {
            MessageBox(NULL, TEXT("This program requires Windows NT!"),
                szAppName, MB_ICONERROR);
            return 0;
        }

        hwnd = CreateWindow(szAppName,
            TEXT("Calculator"),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            NULL,
            NULL,
            hInstance,
            NULL);

        ShowWindow(hwnd, iCmdShow);
        UpdateWindow(hwnd);

        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return msg.wParam;
    }
    /*19'uncu sat�rdan itibaren 63'�nc� sat�ra kadar olan kodpar�as� Microsoft Windows API'si kullan�larak yaz�lm�� bir Windows uygulamas�na aittir
    . Kod, Windows i�letim sistemi taraf�ndan sa�lanan fonksiyonlar� kullanarak bir pencere olu�turan, 
    pencere olaylar�n� i�leyen ve basit bir d�ng� i�inde uygulamay� �al��t�ran bir Windows uygulamas�n�n temelini atmaktad�r.windows API fonksiyonalr�n� 
    kullanarak pencere i�leme fonksiyonunu (WndProc) tan�mlar.
    
    WNDCLASS yap�s� ile bir pencere s�n�f� tan�mlan�r.
    Pencere s�n�f� kaydedilir (RegisterClass).
    Bir pencere olu�turulur (CreateWindow).
    Pencere g�sterilir ve g�ncellenir (ShowWindow, UpdateWindow).
    GetMessage, TranslateMessage, ve DispatchMessage fonksiyonlar� ile uygulaman�n ana d�ng�s� ba�lat�l�r.*/

    LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
        static HWND hEdit;
        static char buffer[256];//kullan�c�n�n giri� yapt��� metni saklamak i�in kullan�l�r.
        static double num1, num2, result;//i�lemler s�ras�nda girilen sayilar� ve sonu�lar� saklar.
        static char operation;//yapacak oldu�umuz i�lem i�in operat�r tan�ml�yoruz.
        static BOOL newInput = TRUE;/*Bu de�i�ken, yeni bir giri�in ba�lad���n� belirlemek i�in kullan�l�r.
        �rne�in, kullan�c� yeni bir say� girmeye ba�lad���nda, bu de�er TRUE olabilir.*/

        char operators[] = { '+', '-', '*', '/' };//operat�rleri tan�mlad�m.

        switch (message) {
        case WM_CREATE:
            hEdit = CreateWindow(TEXT("edit"), NULL,//create window bir pencere a�mak i�in kullan�l�r.
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,//butonun stil �zellikleri belirtilmi�tir.
                10, 10, 240, 25,//butonun koordinatlar�d�r.
                hwnd, (HMENU)1, NULL, NULL);//hmenu1=bir men� tan�mlay�c�s�d�r.di�er iki paremetre ile ilgili �zellik bilinmdei�i i�in null yaz�ld�.

            int buttonX = 10;//X ve Y butonlar� kod �al��t���nda g�r�nmeyen bir butonun koordinatlar�n� belirler.
            int buttonY = 40;

            // Say� tu�lar�
            int i;
            for (i = 0; i < 10; ++i) {//d�ng�n�n her bir ad�m�nda bir buton olu�turulur.
                TCHAR buttonText[2];
                buttonText[0] = '0' + i;
                buttonText[1] = '\0';//buton olu�turma i�lemi sonland�r�l�r.
                CreateWindow(TEXT("button"), buttonText,
                    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                    buttonX, buttonY, 50, 25,
                    hwnd, (HMENU)(i + 2), NULL, NULL);

                if ((i + 1) % 3 == 0) {
                //if blo�u �al���r ise yeni gelecek buton bir alt sat�rdan olu�maya devam eder.aksi taktirde ayn� s�radan devam eder.
                
                    buttonX = 10;
                    buttonY += 30;//alt sat�ra ge�mek i�in y koordinat�na 30 eklenir.
                }
                else {
                    buttonX += 60;//ayn� sat�rdan devam etmek i�in X koordinat�na 60 eklenir.
                }
            }

            // Operat�r tu�lar�
            buttonX = 200;
            buttonY = 40;
            //yukar�daki iki sat�rda i�lemlerin ba�lang�� koordinatlar� belirtilmi�tir.
            for (i = 0; i < 4; ++i) {
                TCHAR buttonText[2];
                buttonText[0] = operators[i];
                buttonText[1] = '\0';//karekter dizisi sonland�r�l�r.

                CreateWindow(TEXT("button"), buttonText,
                    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                    buttonX, buttonY, 50, 25,
                    hwnd, (HMENU)(i + 12), NULL, NULL);

                buttonY += 30;//her buton olu�turulduktan sonra di�er buton i�in y de�eri 30 artt�r�l�r.
            }

            // E�ittir tu�u
            CreateWindow(TEXT("button"), TEXT("="),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                10, 160, 240, 25,
                hwnd, (HMENU)16, NULL, NULL);

            // Clear tu�u
            CreateWindow(TEXT("button"), TEXT("Clear"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//bs_pushbutton=itme d��mesi anlam�na gelir.
                70, 130, 50, 25,
                hwnd, (HMENU)17, NULL, NULL);

            // C tu�u
            CreateWindow(TEXT("button"), TEXT("C"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                130, 130, 50, 25,
                hwnd, (HMENU)18, NULL, NULL);

            break;
            // 135'inci sat�rdan 150'inci sat�ra kadar tu�lar olu�turulmu�tur.koordinatlar� belirlenmi�tir.break ile d��me olu�turma i�lemi bitmi�tir.
        case WM_COMMAND://butonun kullan�c� taraf�ndan etkile�ime girdi�ini belirtir.
            switch (LOWORD(wParam)) {//d��melerin men� tan�mlay�c�lar�na (HMENU) g�re bir dizi durumu ele al�r.
            case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11:
                if (newInput) {//bu kod blo�u newInput de�eri true oldu�u zaman �al���r.bu durum kullan�c�n yeni bir say� giri�i yapt���n� g�sterir.
                    SetWindowText(hEdit, "");
                    newInput = FALSE;
                }
                GetWindowText(hEdit, buffer, sizeof(buffer));//Edit kontrol�n�n i�eri�i buffer adl� karakter dizisine (char array) al�n�r.
                sprintf(buffer, "%s%c", buffer, '0' + LOWORD(wParam) - 2);
            // sprintf fonksiyonu, buffer dizisine formatl� bir �ekilde metin ekler. Bu durumda, buffer'a, mevcut i�eri�in sonuna (%s) t�klanan d��menin say�s�n� ekler ('0' + LOWORD(wParam) - 2).
            //LOWORD(wParam) - 2 ifadesi, t�klanan d��menin say�s�n� temsil eder. �rne�in, 2 numaral� d��me t�kland���nda, bu ifade 0 de�erini al�r, ��nk� 2 - 2 = 0.   
                
                SetWindowText(hEdit, buffer);//Edit kontrol�n�n i�eri�i, g�ncellenmi� buffer ile de�i�tirilir.
                break;

            case 12: case 13: case 14: case 15:
                GetWindowText(hEdit, buffer, sizeof(buffer));//Edit kontrol�n�n i�eri�i buffer adl� karakter dizisine (char array) al�n�r.
                num1 = atof(buffer);//atof fonksiyonu, bir karakter dizisini (buffer) double t�r�ne d�n��t�rerek num1 de�i�kenine atar. Bu, kullan�c�n�n girdi�i say�y� temsil eder.
                operation = operators[LOWORD(wParam) - 12];//operators adl� bir dizi, 12 ile 15 aras�ndaki d��melerin s�ras�yla temsil etti�i matematiksel operat�rleri i�erir.
                newInput = TRUE;//newInput de�i�keni TRUE olarak ayarlan�r. Bu, yeni bir say� giri�i ba�lad���n� belirtir.
                break;

            case 16:
                GetWindowText(hEdit, buffer, sizeof(buffer));
                if (buffer[0] != '\0') {//E�er buffer'�n ilk karakteri null karakter ('\0') de�ilse, yani edit kontrol�nde bir say� varsa, bu blok i�ine girer.
                    num2 = atof(buffer);//atof fonksiyonu, bir karakter dizisini (buffer) double t�r�ne d�n��t�rerek num2 de�i�kenine atar. Bu, kullan�c�n�n girdi�i ikinci say�y� temsil eder.
                    switch (operation) {//hangi operat�r butonuna t�klan�rsa belirlenen matemetiksel i�lem ger�ekle�tirilir
                    case '+':
                        result = num1 + num2;
                        break;
                    case '-':
                        result = num1 - num2;
                        break;
                    case '*':
                        result = num1 * num2;
                        break;
                    case '/':
                        if (num2 != 0) {
                            result = num1 / num2;
                        }
                        else {
                            MessageBox(hwnd, "Cannot divide by zero", "Error", MB_OK | MB_ICONERROR);//E�er num2 s�f�rsa, bir hata mesaj� g�r�nt�lenir ve i�lem sonland�r�l�r.
                            break;
                        }
                        break;
                    default://Yukar�daki durumlar�n hi�birine uymayan bir durum varsa (ge�erli bir operat�r belirlenemediyse), bu blok �al���r ve i�lem sonland�r�l�r.
                        break;
                    }

                    
                    snprintf(buffer, sizeof(buffer), "%.6f", result);// Virg�lden sonra en fazla 6 basamak g�ster

                    // Gereksiz s�f�rlar� temizlemek i�in a�a��daki fonksiyonu kullan�r�z.
                    removeTrailingZeros(buffer);

                    SetWindowText(hEdit, buffer);
                    newInput = TRUE;
                }
                break;

            case 17: // Clear
                SetWindowText(hEdit, "");//Edit kontrol�n�n i�eri�ini temizler. Yani, penceredeki metin kutusundaki her �eyi siler.
                num1 = num2 = result = 0.0;//say�lar s�f�rlan�r.
                operation = '\0';//hesap makinesinde kullan�lan operat�rler s�f�rlan�r.
                newInput = TRUE;//newInput de�i�kenini TRUE olarak ayarlar. Bu, bir sonraki kullan�c�n�n giri�inin yeni bir say� giri�i oldu�unu belirtir.
                break;

            case 18: // C
                GetWindowText(hEdit, buffer, sizeof(buffer));//kullan�c�n�n girdi�i metni i�erir.
                int length = strlen(buffer);//buffer dizisinin uzunlu�unu hesaplar. strlen fonksiyonu, null karakter ('\0') bulana kadar olan karakter say�s�n� d�nd�r�r.
                if (length > 0) {//E�er buffer'�n uzunlu�u (length) 0'dan b�y�kse, yani i�eride metin varsa, bu blo�a girer.
                    buffer[length - 1] = '\0';//buffer dizisinin son karakterini (buffer[length - 1]) null karakter ('\0') ile de�i�tirir. Bu, en son karakteri siler.
                    SetWindowText(hEdit, buffer);//Edit kontrol�ne (hEdit) g�ncellenmi� buffer'� yazarak i�eri�i g�nceller. Bu sayede en son karakter silinmi� olur.
                }
                break;//��teki switch blo�undan ��kar. Bu, i�teki switch-case yap�s�n� sonland�r�r
            }
            break;//d��taki switch blo�undan ��kar. Bu, d��taki switch-case yap�s�n� sonland�r�r.

        case WM_DESTROY:
            PostQuitMessage(0);//Windows'a uygulaman�n sonland�r�lmas�n� bildiren bir mesaj g�nderir. Bu, uygulaman�n kapat�lmas�na neden olur.
            break;//Bu, WM_DESTROY durumunu i�ledikten sonra switch-case yap�s�ndan ��k�� yapar.

        default:
            return DefWindowProc(hwnd, message, wParam, lParam);//bilinmeyen bir mesaj t�r� i�lendi�inde olay� varsay�lan windoesun pencere i�leme fonksiyonuna y�nlendirilir.
        }

        return 0;
    }

    // Gereksiz s�f�rlar� temizleme fonksiyonu
    void removeTrailingZeros(char* str) {
        int length = strlen(str);//str karakter dizisinin uzunlu�unu hesaplar.
        while (length > 0 && str[length - 1] == '0') {//bu d�ng� dizinin sonundan ba�layarak s�f�r karakterlerini bulur ve her birini null karakter ('\0') ile de�i�tirir. Bu �ekilde, gereksiz s�f�rlar kald�r�l�r.
            str[--length] = '\0';
        }
        if (length > 0 && str[length - 1] == '.') {
            //E�er dizinin son karakteri bir ondal�k noktaysa, bu ondal�k noktay� kald�r�r.D�ng�, length'i g�nceller ve ondal�k nokta kald�r�ld�k�a devam eder.
            str[--length] = '\0';
        }
    }

