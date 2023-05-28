# steganography
hiding information in a image by changing the image's LSB..
Steganography is the art of hiding the fact that communication is taking place, by hiding information in other information. 
Many carrier file formats can be used, but digital images are the most popular because of their frequency on the internet.
For hiding secret information in images, there exists a large variety of steganography techniques some are more complex than others and all of them have respective strong and weak points. 
Different applications may require absolute invisibility of the secret information, while others require a large secret message to be hidden.



Requirement Details

The application accepts an image file say .bmp along with a text file that contains the message to be steged
Analyze the size of the message file to check whether the message could fit in the provided .bmp image
Provide an option to steg a magic string which could be useful to identify whether the image is steged or not
The application should provide an option to decrypt the file
This has to be a command-line application and all the options have to be passed as a command-line argument
