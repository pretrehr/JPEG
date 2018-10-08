
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<stdbool.h>
#include <bitstream.h>



struct bitstream{
  uint8_t buffer;
  FILE *fichier;
  int8_t bit_courant;
};

extern struct bitstream * bitstream_create ( const char * filename )
{
 FILE *file = fopen(filename, "w");
 uint8_t buffer = 0;
 uint8_t bit_courant = 0;
 struct bitstream *btstrm = malloc(sizeof(struct bitstream));
 btstrm->buffer = buffer;
 btstrm->fichier = file;
 btstrm->bit_courant = bit_courant;
 return btstrm;
}


extern void bitstream_write_nbits ( struct bitstream *stream ,
                                              uint32_t value ,
                                              uint8_t nbits ,
                                              bool is_marker )
{
  uint32_t position;
  for(int i=nbits-1; i>=0; i--)
  {   position = ((1<<(i))&value);
      position = position>>i;
      stream->buffer = (stream->buffer<<1)|position;
      stream->bit_courant++;
      if(stream->bit_courant == 8)
      {
        fwrite(&stream->buffer, 1, 1, stream->fichier);
        if(stream->buffer ==0xff && !is_marker)
        {
          stream->buffer = 0;
          fwrite(&stream->buffer, 1, 1, stream->fichier);
          stream->bit_courant = 0;
        }
        else
        {
          stream->buffer = 0;
          stream->bit_courant = 0;
        }
      }
  }
}


extern void bitstream_flush ( struct bitstream * stream )
{
  if (stream->bit_courant != 0)
  {
    stream->buffer = stream->buffer<<(8-stream->bit_courant);
    fwrite(&stream->buffer, 1, 1, stream->fichier);
    stream->buffer = 0;
    stream->bit_courant = 0;
  }

}


extern void bitstream_destroy ( struct bitstream * stream )
{
  fclose(stream->fichier);
  free(stream);
}

#ifdef TEST_BITSTREAM
int main(void)
{
  uint32_t val = 7;//0000 0111
  uint32_t val2 = 59; //0011 1011
  uint32_t val3 = 255//1111 1111
  struct bitstream *btstrm = bitstream_create("hello.txt");
  bitstream_write_nbits(btstrm, val2, 8, 1);
  bitstream_write_nbits(btstrm, val3, 8, 1);
  bitstream_write_nbits(btstrm, val3, 8, 0);
  bitstream_write_nbits(btstrm, val, 3, 1);
  bitstream_flush(btstrm);
  bitstream_write_nbits(btstrm, val, 4, 1);
  bitstream_destroy(btstrm);
}
#endif
