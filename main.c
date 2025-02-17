    #include <windows.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <tchar.h>/* bu kütüphane  Microsoft Visual Studio'nun çoklu karakter seti (MBCS) ve geniþ karakter seti (Unicode) gibi farklý karakter 
    setleri arasýnda geçiþ yapmaya yardýmcý olan bir dizi makro tanýmlar. Bu, yazýlým geliþtiricilere,
    kodlarýný farklý karakter setlerinde derlemek için daha fazla esneklik saðlar. */

    LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);//Bu satýr, Windows uygulamalarýnda bir pencere iþleme fonksiyonunu  tanýmlar. 
    /*
    HWND hWnd: Ýþlemin baðlý olduðu pencerenin tanýmlayýcýsý (handle).
    UINT message: Ýþlenen olayýn türünü belirten bir sayýsal deðer.
    WPARAM wParam: Ýþleme geçirilen ek bilgiler, genellikle olaya özgü veri.
    LPARAM lParam: Ýþleme geçirilen ek bilgiler, genellikle olaya özgü veri.
    Fonksiyonun dönüþ deðeri LRESULT türündedir ve genellikle olaya özgü bir deðer içerir. */

    void removeTrailingZeros(char* str);//bu fonksiyon iþlemin sonucundaki gereksiz sýfýrlarý silmemize yardýmcý olur.

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
    /*19'uncu satýrdan itibaren 63'üncü satýra kadar olan kodparçasý Microsoft Windows API'si kullanýlarak yazýlmýþ bir Windows uygulamasýna aittir
    . Kod, Windows iþletim sistemi tarafýndan saðlanan fonksiyonlarý kullanarak bir pencere oluþturan, 
    pencere olaylarýný iþleyen ve basit bir döngü içinde uygulamayý çalýþtýran bir Windows uygulamasýnýn temelini atmaktadýr.windows API fonksiyonalrýný 
    kullanarak pencere iþleme fonksiyonunu (WndProc) tanýmlar.
    
    WNDCLASS yapýsý ile bir pencere sýnýfý tanýmlanýr.
    Pencere sýnýfý kaydedilir (RegisterClass).
    Bir pencere oluþturulur (CreateWindow).
    Pencere gösterilir ve güncellenir (ShowWindow, UpdateWindow).
    GetMessage, TranslateMessage, ve DispatchMessage fonksiyonlarý ile uygulamanýn ana döngüsü baþlatýlýr.*/

    LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
        static HWND hEdit;
        static char buffer[256];//kullanýcýnýn giriþ yaptýðý metni saklamak için kullanýlýr.
        static double num1, num2, result;//iþlemler sýrasýnda girilen sayilarý ve sonuçlarý saklar.
        static char operation;//yapacak olduðumuz iþlem için operatör tanýmlýyoruz.
        static BOOL newInput = TRUE;/*Bu deðiþken, yeni bir giriþin baþladýðýný belirlemek için kullanýlýr.
        Örneðin, kullanýcý yeni bir sayý girmeye baþladýðýnda, bu deðer TRUE olabilir.*/

        char operators[] = { '+', '-', '*', '/' };//operatörleri tanýmladým.

        switch (message) {
        case WM_CREATE:
            hEdit = CreateWindow(TEXT("edit"), NULL,//create window bir pencere açmak için kullanýlýr.
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,//butonun stil özellikleri belirtilmiþtir.
                10, 10, 240, 25,//butonun koordinatlarýdýr.
                hwnd, (HMENU)1, NULL, NULL);//hmenu1=bir menü tanýmlayýcýsýdýr.diðer iki paremetre ile ilgili özellik bilinmdeiði için null yazýldý.

            int buttonX = 10;//X ve Y butonlarý kod çalýþtýðýnda görünmeyen bir butonun koordinatlarýný belirler.
            int buttonY = 40;

            // Sayý tuþlarý
            int i;
            for (i = 0; i < 10; ++i) {//döngünün her bir adýmýnda bir buton oluþturulur.
                TCHAR buttonText[2];
                buttonText[0] = '0' + i;
                buttonText[1] = '\0';//buton oluþturma iþlemi sonlandýrýlýr.
                CreateWindow(TEXT("button"), buttonText,
                    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                    buttonX, buttonY, 50, 25,
                    hwnd, (HMENU)(i + 2), NULL, NULL);

                if ((i + 1) % 3 == 0) {
                //if bloðu çalýþýr ise yeni gelecek buton bir alt satýrdan oluþmaya devam eder.aksi taktirde ayný sýradan devam eder.
                
                    buttonX = 10;
                    buttonY += 30;//alt satýra geçmek için y koordinatýna 30 eklenir.
                }
                else {
                    buttonX += 60;//ayný satýrdan devam etmek için X koordinatýna 60 eklenir.
                }
            }

            // Operatör tuþlarý
            buttonX = 200;
            buttonY = 40;
            //yukarýdaki iki satýrda iþlemlerin baþlangýç koordinatlarý belirtilmiþtir.
            for (i = 0; i < 4; ++i) {
                TCHAR buttonText[2];
                buttonText[0] = operators[i];
                buttonText[1] = '\0';//karekter dizisi sonlandýrýlýr.

                CreateWindow(TEXT("button"), buttonText,
                    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                    buttonX, buttonY, 50, 25,
                    hwnd, (HMENU)(i + 12), NULL, NULL);

                buttonY += 30;//her buton oluþturulduktan sonra diðer buton için y deðeri 30 arttýrýlýr.
            }

            // Eþittir tuþu
            CreateWindow(TEXT("button"), TEXT("="),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                10, 160, 240, 25,
                hwnd, (HMENU)16, NULL, NULL);

            // Clear tuþu
            CreateWindow(TEXT("button"), TEXT("Clear"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//bs_pushbutton=itme düðmesi anlamýna gelir.
                70, 130, 50, 25,
                hwnd, (HMENU)17, NULL, NULL);

            // C tuþu
            CreateWindow(TEXT("button"), TEXT("C"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                130, 130, 50, 25,
                hwnd, (HMENU)18, NULL, NULL);

            break;
            // 135'inci satýrdan 150'inci satýra kadar tuþlar oluþturulmuþtur.koordinatlarý belirlenmiþtir.break ile düðme oluþturma iþlemi bitmiþtir.
        case WM_COMMAND://butonun kullanýcý tarafýndan etkileþime girdiðini belirtir.
            switch (LOWORD(wParam)) {//düðmelerin menü tanýmlayýcýlarýna (HMENU) göre bir dizi durumu ele alýr.
            case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11:
                if (newInput) {//bu kod bloðu newInput deðeri true olduðu zaman çalýþýr.bu durum kullanýcýn yeni bir sayý giriþi yaptýðýný gösterir.
                    SetWindowText(hEdit, "");
                    newInput = FALSE;
                }
                GetWindowText(hEdit, buffer, sizeof(buffer));//Edit kontrolünün içeriði buffer adlý karakter dizisine (char array) alýnýr.
                sprintf(buffer, "%s%c", buffer, '0' + LOWORD(wParam) - 2);
            // sprintf fonksiyonu, buffer dizisine formatlý bir þekilde metin ekler. Bu durumda, buffer'a, mevcut içeriðin sonuna (%s) týklanan düðmenin sayýsýný ekler ('0' + LOWORD(wParam) - 2).
            //LOWORD(wParam) - 2 ifadesi, týklanan düðmenin sayýsýný temsil eder. Örneðin, 2 numaralý düðme týklandýðýnda, bu ifade 0 deðerini alýr, çünkü 2 - 2 = 0.   
                
                SetWindowText(hEdit, buffer);//Edit kontrolünün içeriði, güncellenmiþ buffer ile deðiþtirilir.
                break;

            case 12: case 13: case 14: case 15:
                GetWindowText(hEdit, buffer, sizeof(buffer));//Edit kontrolünün içeriði buffer adlý karakter dizisine (char array) alýnýr.
                num1 = atof(buffer);//atof fonksiyonu, bir karakter dizisini (buffer) double türüne dönüþtürerek num1 deðiþkenine atar. Bu, kullanýcýnýn girdiði sayýyý temsil eder.
                operation = operators[LOWORD(wParam) - 12];//operators adlý bir dizi, 12 ile 15 arasýndaki düðmelerin sýrasýyla temsil ettiði matematiksel operatörleri içerir.
                newInput = TRUE;//newInput deðiþkeni TRUE olarak ayarlanýr. Bu, yeni bir sayý giriþi baþladýðýný belirtir.
                break;

            case 16:
                GetWindowText(hEdit, buffer, sizeof(buffer));
                if (buffer[0] != '\0') {//Eðer buffer'ýn ilk karakteri null karakter ('\0') deðilse, yani edit kontrolünde bir sayý varsa, bu blok içine girer.
                    num2 = atof(buffer);//atof fonksiyonu, bir karakter dizisini (buffer) double türüne dönüþtürerek num2 deðiþkenine atar. Bu, kullanýcýnýn girdiði ikinci sayýyý temsil eder.
                    switch (operation) {//hangi operatör butonuna týklanýrsa belirlenen matemetiksel iþlem gerçekleþtirilir
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
                            MessageBox(hwnd, "Cannot divide by zero", "Error", MB_OK | MB_ICONERROR);//Eðer num2 sýfýrsa, bir hata mesajý görüntülenir ve iþlem sonlandýrýlýr.
                            break;
                        }
                        break;
                    default://Yukarýdaki durumlarýn hiçbirine uymayan bir durum varsa (geçerli bir operatör belirlenemediyse), bu blok çalýþýr ve iþlem sonlandýrýlýr.
                        break;
                    }

                    
                    snprintf(buffer, sizeof(buffer), "%.6f", result);// Virgülden sonra en fazla 6 basamak göster

                    // Gereksiz sýfýrlarý temizlemek için aþaðýdaki fonksiyonu kullanýrýz.
                    removeTrailingZeros(buffer);

                    SetWindowText(hEdit, buffer);
                    newInput = TRUE;
                }
                break;

            case 17: // Clear
                SetWindowText(hEdit, "");//Edit kontrolünün içeriðini temizler. Yani, penceredeki metin kutusundaki her þeyi siler.
                num1 = num2 = result = 0.0;//sayýlar sýfýrlanýr.
                operation = '\0';//hesap makinesinde kullanýlan operatörler sýfýrlanýr.
                newInput = TRUE;//newInput deðiþkenini TRUE olarak ayarlar. Bu, bir sonraki kullanýcýnýn giriþinin yeni bir sayý giriþi olduðunu belirtir.
                break;

            case 18: // C
                GetWindowText(hEdit, buffer, sizeof(buffer));//kullanýcýnýn girdiði metni içerir.
                int length = strlen(buffer);//buffer dizisinin uzunluðunu hesaplar. strlen fonksiyonu, null karakter ('\0') bulana kadar olan karakter sayýsýný döndürür.
                if (length > 0) {//Eðer buffer'ýn uzunluðu (length) 0'dan büyükse, yani içeride metin varsa, bu bloða girer.
                    buffer[length - 1] = '\0';//buffer dizisinin son karakterini (buffer[length - 1]) null karakter ('\0') ile deðiþtirir. Bu, en son karakteri siler.
                    SetWindowText(hEdit, buffer);//Edit kontrolüne (hEdit) güncellenmiþ buffer'ý yazarak içeriði günceller. Bu sayede en son karakter silinmiþ olur.
                }
                break;//Ýçteki switch bloðundan çýkar. Bu, içteki switch-case yapýsýný sonlandýrýr
            }
            break;//dýþtaki switch bloðundan çýkar. Bu, dýþtaki switch-case yapýsýný sonlandýrýr.

        case WM_DESTROY:
            PostQuitMessage(0);//Windows'a uygulamanýn sonlandýrýlmasýný bildiren bir mesaj gönderir. Bu, uygulamanýn kapatýlmasýna neden olur.
            break;//Bu, WM_DESTROY durumunu iþledikten sonra switch-case yapýsýndan çýkýþ yapar.

        default:
            return DefWindowProc(hwnd, message, wParam, lParam);//bilinmeyen bir mesaj türü iþlendiðinde olayý varsayýlan windoesun pencere iþleme fonksiyonuna yönlendirilir.
        }

        return 0;
    }

    // Gereksiz sýfýrlarý temizleme fonksiyonu
    void removeTrailingZeros(char* str) {
        int length = strlen(str);//str karakter dizisinin uzunluðunu hesaplar.
        while (length > 0 && str[length - 1] == '0') {//bu döngü dizinin sonundan baþlayarak sýfýr karakterlerini bulur ve her birini null karakter ('\0') ile deðiþtirir. Bu þekilde, gereksiz sýfýrlar kaldýrýlýr.
            str[--length] = '\0';
        }
        if (length > 0 && str[length - 1] == '.') {
            //Eðer dizinin son karakteri bir ondalýk noktaysa, bu ondalýk noktayý kaldýrýr.Döngü, length'i günceller ve ondalýk nokta kaldýrýldýkça devam eder.
            str[--length] = '\0';
        }
    }

