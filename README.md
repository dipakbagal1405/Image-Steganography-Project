# Image Steganography (Hide Text in Image)

A console-based application implemented in C to securely hide secret text messages inside BMP (Bitmap) images using **LSB (Least Significant Bit) Steganography**. It also provides a decoding mechanism to safely extract the hidden message back from the stego-image.

## 🚀 Features
* **Encode Mode:** Embeds a secret text file inside a source `.bmp` image without visibly altering its quality.
* **Decode Mode:** Extracts the hidden secret message from a stego `.bmp` image and outputs it into a text file.
* **Size Validation:** Automatically checks if the target image is large enough to safely hold the secret text data.

## 🛠️ Tech Stack & Key Concepts
* **Language:** C Programming
* **Concepts:** File I/O, Bitwise Operations, Structs, BMP Image File Structure (Header parsing).

## 💻 How to Run
Compile all the source files using GCC and run the executable via terminal:

1. **Compilation:**
   ```bash
   gcc test_encode.c encode.c decode.c -o stego
   ```

2. **To Encode (Hide Message):**
   ```bash
   ./stego -e beautiful.bmp secret.txt stego.bmp
   ```

3. **To Decode (Extract Message):**
   ```bash
   ./stego -d stego.bmp output.txt
   ```
