Help Menu
```
./image2tiff
input-files or input-dir is required
 - Image to Tiff conversion program
Usage:
  image2tiff [OPTION...]

      --debug             Enable debugging
      --input-files FILE  Files to combine
      --input-dir DIR     Input directory of images to combine
      --output FILE       Output file (default: generated_ts.tiff)
      --help              Print help

 Output options:
      --bitonal     Convert to bitonal (default: true)
      --dpi N       Output resolution (600, 300, 150, 96, 72) (default: 300)
      --blank-page  Create blank-pages when conversion fails for input image
                    (default: true)

```


Example : Convert and aggregate into tiff

```
--input-dir=~/dev/image2tiff/test-deck/deck-001/ --output=/mnt/hgfs/vm-mnt/image2tiff/final-01.tiff
```