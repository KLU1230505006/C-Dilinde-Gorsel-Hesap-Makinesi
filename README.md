Windows Hesap Makinesi Uygulaması
Bu proje, Windows API kullanılarak geliştirilmiş basit bir hesap makinesi uygulamasını içerir. Uygulama, temel matematiksel işlemleri (toplama, çıkarma, çarpma, bölme) gerçekleştirebilir ve kullanıcı dostu bir arayüz sunar.

Özellikler
Temel İşlemler: Toplama (+), çıkarma (-), çarpma (*), bölme (/) işlemlerini destekler.

Clear (Temizle): Ekrandaki tüm içeriği temizler.

C (Sil): Son girilen karakteri siler.

Ondalık Sayı Desteği: Ondalık sayılar üzerinde işlem yapabilir ve gereksiz sıfırları otomatik olarak kaldırır.

Kullanıcı Dostu Arayüz: Butonlar ve metin kutusu ile kolay kullanım sağlar.

Nasıl Çalışır?
Uygulama, Windows API'si kullanılarak oluşturulmuş bir pencere içinde çalışır. Kullanıcı, sayıları ve operatörleri butonlar aracılığıyla girer ve sonuç, ekranda görüntülenir. İşlemler, kullanıcının girdiği sayılar ve seçtiği operatörlere göre gerçekleştirilir.

Kod Yapısı
WinMain: Uygulamanın ana giriş noktasıdır. Pencere sınıfını kaydeder, pencereyi oluşturur ve mesaj döngüsünü başlatır.

WndProc: Pencere mesajlarını işler. Buton tıklamaları ve diğer olaylar bu fonksiyon tarafından ele alınır.

removeTrailingZeros: Ondalık sayılardaki gereksiz sıfırları kaldıran yardımcı bir fonksiyondur.

Nasıl Kullanılır?
Derleme: Bu kodu bir C derleyicisi (örneğin, Microsoft Visual Studio) ile derleyin.

Çalıştırma: Derlenen uygulamayı çalıştırın.

Kullanım:

Sayıları girmek için ekrandaki sayı butonlarını kullanın.

İşlem yapmak için operatör butonlarını (+, -, *, /) kullanın.

Sonucu görmek için "=" butonuna basın.

Ekranı temizlemek için "Clear" butonunu kullanın.

Son girilen karakteri silmek için "C" butonunu kullanın.

Örnek Kullanım
5 tuşuna basın.

+ tuşuna basın.

3 tuşuna basın.

= tuşuna basın.

Sonuç: 8 ekranda görüntülenecektir.
