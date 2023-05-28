# steganography
hiding information in a image by changing the image's LSB..
Steganography is the art of hiding the fact that communication is taking place, by hiding information in other information. 
Many carrier file formats can be used, but digital images are the most popular because of their frequency on the internet.
For hiding secret information in images, there exists a large variety of steganography techniques some are more complex than others and all of them have respective strong and weak points. 
Different applications may require absolute invisibility of the secret information, while others require a large secret message to be hidden.

Requirement Details:
The application accepts an image file say .bmp along with a text file that contains the message to be steged
Analyze the size of the message file to check whether the message could fit in the provided .bmp image
Provide an option to steg a magic string which could be useful to identify whether the image is steged or not
The application should provide an option to decrypt the file
This has to be a command-line application and all the options have to be passed as a command-line argument

Usage: Encoding: ./a.out -e <.bmp_file> <.txt_file> [output file]
Usage: Decoding: ./a.out -d <.bmp_file> [output file]

Approach: Digital image steganography is used to hide confidential data within an image. Least significant bit (LSB) is a well-known steganography approach. LSB stegenography method is used to encode a message such a way that the quality of the image doesn't change much.

We take 1 byte of image data and encode/modify only the LSB bit. Accordingly, to encode 1 byte of text data, we would need 8 bytes of image data.

We use BMP files because typically, these image files are uncompressed, and therefore large and lossless. Their advantage is their simple structure and wide acceptance in Windows programs.

While encoding, we would need to skip the first 54 bytes of header data in .bmp file so that the metadata wouldn't change and modify image properties accidentaly. Here we start encoding from the 55th byte.
