# RoPilot v1.0.3 Changelog

## ✨ New Features & Improvements

- **Pembaruan Pengaturan Tema (Appearance Update)**: 
  - Ditambahkan fitur untuk mengubah **Bahasa** (Language) aplikasi, dengan dukungan terjemahan penuh untuk Bahasa Indonesia dan Inggris.
  - Ditambahkan fitur **Skala UI** (UI Scale) berupa *slider* interaktif untuk menyesuaikan ukuran antarmuka dari Kecil (80%), Sedang (100%), hingga Besar (120%) secara instan tanpa ada *flash* saat aplikasi dimulai ulang.
  - Ditambahkan pengaturan **Font** (Font Family) untuk mengubah jenis huruf UI secara menyeluruh dengan sistem pratinjau (*live preview*).
  - Ditambahkan pengaturan **Window Opacity** dan **Background Blur** secara native (*Acrylic*) untuk menyesuaikan tingkat transparansi jendela utama tanpa mempengaruhi keterbacaan elemen dialog (modal).
- **Smart Cookie Validation**: Sistem otomatis mengecek akun secara berkala di latar belakang. Jika *cookie* kedaluwarsa (401), antarmuka akan menampilkan status "Invalid Cookie" lengkap dengan indikator merah dan tombol "Re-Login".
- **Add Account Modal Revamp**: Desain baru untuk antarmuka penambahan akun (via Browser & Cookie) yang menggunakan *modal pop-up* terpusat dengan animasi transisi yang mulus, dilengkapi dengan kotak peringatan keamanan *cookie*.
- **Hide Username & User ID**: Fitur untuk menyembunyikan username dan user ID akun Anda (Dapat dinyalakan melalui Settings).
- **Fixed Avatar Delay**: Added background polling to handle the delay in Roblox's backend updating CDN thumbnail URLs, ensuring the UI reflects the current outfit correctly.
- **Pembaruan Manajemen Akun (Utility Dashboard)**:
  - **Account Overview**: Menampilkan informasi detail seperti umur akun dan status premium (Roblox Plus).
  - **Display Name Editing**: Mengubah *Display Name* akun secara langsung melalui aplikasi.
  - **Outfit Changer**: Memuat seluruh daftar *outfit* dan menggantinya secara instan.
  - **Roblox Experience**: Menampilkan riwayat game, meluncurkan game via Place ID / Tautan Server Pribadi dengan validasi otomatis, serta fitur pintar *auto-kill* instance lama.
  - **Social**: Menampilkan seluruh daftar teman yang sedang *online*, dan mendukung fitur **Bulk Unfriend** untuk menghapus beberapa teman secara massal.
  - **Account Settings**: Mengatur untuk join ke Low Server dan menggunakan grafik paling rendah
  
### Bug Fixes
- **Private Server Access**: Fixed an issue where joining private servers via share links (`share?code=...`) or private server codes would result in a "You do not have access" error. RoPilot now correctly fetches the `AccessCode` (Job ID) beforehand, seamlessly replicating the Roblox client's native behavior.
- **Outfit Loading Pagination**: Fixed an issue where only the first 50 outfits were displayed. The system now automatically fetches all available outfits across multiple pages.
- **Outfit Switching Speed**: Significantly sped up outfit changing speed (reduced from multiple seconds to near instant) by optimizing the internal Roblox API calls using parallelized `std::thread` processing.