# RoPilot v1.0.3 Changelog

## ✨ New Features & Improvements

- **Pembaruan Pengaturan Tema (Appearance Update)**: 
  - Ditambahkan fitur untuk mengubah **Bahasa** (Language) aplikasi, dengan dukungan terjemahan penuh untuk Bahasa Indonesia dan Inggris.
  - Ditambahkan fitur **Skala UI** (UI Scale) berupa *slider* interaktif untuk menyesuaikan ukuran antarmuka dari Kecil (80%), Sedang (100%), hingga Besar (120%) secara instan tanpa ada *flash* saat aplikasi dimulai ulang.
  - Ditambahkan pengaturan **Font** (Font Family) untuk mengubah jenis huruf UI secara menyeluruh dengan sistem pratinjau (*live preview*).
  - Ditambahkan pengaturan **Window Opacity** dan **Background Blur** secara native (*Acrylic*) untuk menyesuaikan tingkat transparansi jendela utama tanpa mempengaruhi keterbacaan elemen dialog (modal).
- **Status Persistence**: Status buka/tutup pada *Sidebar* (menu samping) sekarang akan disimpan dan diingat saat aplikasi ditutup dan dijalankan kembali.
- **Smart Cookie Validation**: Sistem otomatis mengecek akun secara berkala di latar belakang. Jika *cookie* kedaluwarsa (401), antarmuka akan menampilkan status "Invalid Cookie" lengkap dengan indikator merah dan tombol "Re-Login".
- **In-Place Re-Login**: Fitur *Re-Login* akan memperbarui *cookie* akun Anda secara langsung tanpa memindahkan/mengeluarkan akun tersebut dari pengaturan Grupnya.
- **Add Account Modal Revamp**: Desain baru untuk antarmuka penambahan akun (via Browser & Cookie) yang menggunakan *modal pop-up* terpusat dengan animasi transisi yang mulus, dilengkapi dengan kotak peringatan keamanan *cookie*.
- **Hide Username & User ID**: Fitur untuk menyembunyikan username dan user ID akun Anda (Dapat dinyalakan melalui Settings)