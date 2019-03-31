# rest_server
Langkah-langkan untuk build:
1. Build config dengan CMake
2. Build aplikasi dengan make
3. Pastikan mongodb sudah running
4. Buat unique index
db.getCollection('users').createIndex({username:1}, {unique: true, name: "_username_"});
5. Buat text index
db.getCollection('users').createIndex({username:"text", firstName:"text", lastName:"text"}, {name: "_user_text_"});
