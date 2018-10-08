#include <stdint.h>
#include <stdlib.h>
#include <bitstream.h>
#include <huffman.h>
#include <htables.h>

/* Structure non opaque contenant toutes les informations nécessaires à
 * la création d'un entête JPEG et un buffer bitstream permettant de stocker 
 * ces informations avant de les écire dans un fichier
 */
struct jpeg_desc
{
    struct bitstream *stream;
    const char *ppm_filename;
    const char *jpeg_filename;
    uint32_t image_height;
    uint32_t image_width;
    uint8_t nb_components;
    uint8_t sampling_factor[NB_COLOR_COMPONENTS][NB_DIRECTIONS];
    struct huff_table *htable[NB_SAMPLE_TYPES][NB_COLOR_COMPONENTS];
    uint8_t *qtable[4];
};



/*********
 *  SET  *
 *********/



/* Alloue et retourne une nouvelle structure jpeg_desc. */

struct jpeg_desc *jpeg_desc_create(void)
{
    struct jpeg_desc *jpeg = malloc(sizeof(struct jpeg_desc));
                            
    return jpeg;
}


/* Ecrit le nom de fichier PPM ppm_filename dans le jpeg_desc jdesc. */
void jpeg_desc_set_ppm_filename(struct jpeg_desc *jdesc, const char *ppm_filename)
{

    jdesc->ppm_filename = ppm_filename;
}

/* Ecrit le nom du fichier de sortie jpeg_filename dans le jpeg_desc jdesc. */
void jpeg_desc_set_jpeg_filename(struct jpeg_desc *jdesc, const char *jpeg_filename)
{
    struct bitstream *stream = bitstream_create(jpeg_filename);
    jdesc->stream = stream;
    jdesc->jpeg_filename = jpeg_filename;
}


/*
    Ecrit la hauteur en nombre de pixels de l'image traitée image_height dans
    le jpeg_desc jdesc.
*/
void jpeg_desc_set_image_height(struct jpeg_desc *jdesc, uint32_t image_height)
{
    jdesc->image_height = image_height;
}


/*
    Ecrit la largeur en nombre de pixels de l'image traitée image_width dans le
    jpeg_desc jdesc.
*/
void jpeg_desc_set_image_width(struct jpeg_desc *jdesc,
                                      uint32_t image_width)
{
    jdesc->image_width = image_width;
}


/*
    Ecrit le nombre de composantes de couleur de l'image traitée nb_components
    dans le jpeg_desc jdesc.
*/
void jpeg_desc_set_nb_components(struct jpeg_desc *jdesc,
                                        uint8_t nb_components)
{
    jdesc->nb_components = nb_components;
}


/*
    Ecrit dans le jpeg_desc jdesc le facteur d'échantillonnage sampling_factor
    à utiliser pour la composante de couleur cc et la direction dir.
*/
void jpeg_desc_set_sampling_factor(struct jpeg_desc *jdesc,
                                          enum color_component cc,
                                          enum direction dir,
                                          uint8_t sampling_factor)
{
    jdesc->sampling_factor[cc][dir] = sampling_factor;
}


/*
    Ecrit dans le jpeg_desc jdesc la table de Huffman huff_table à utiliser
    pour encoder les données de la composante fréquentielle acdc pour la
    composante de couleur cc.
*/
void jpeg_desc_set_huffman_table(struct jpeg_desc *jdesc,
                                        enum sample_type acdc,
                                        enum color_component cc,
                                        struct huff_table *htable)
{
    jdesc->htable[acdc][cc] = htable;
}

/*
    Ecrit dans le jpeg_desc jdesc la table de quantification qtable à utiliser
    pour compresser les coefficients de la composante de couleur cc.
*/
void jpeg_desc_set_quantization_table(struct jpeg_desc *jdesc,
                                             enum color_component cc,
                                             uint8_t *qtable)
{
    jdesc->qtable[cc] = qtable;
}


/***********************************************/
/* Ouverture, fermeture et fonctions générales */
/***********************************************/

/*
    Ecrit toute l'entête JPEG dans le fichier de sortie à partir des
    informations contenues dans le jpeg_desc passé en paramètre. En sortie, le
    bitstream est positionné juste après l'écriture de l'entête SOS, à
    l'emplacement du premier octet de données brutes à écrire.
*/
void jpeg_write_header(struct jpeg_desc *jdesc)
{

    //Marqueur de début d'image SOI
    bitstream_write_nbits(jdesc->stream, 0xffd8, 16, true);

    //Application data
    bitstream_write_nbits(jdesc->stream, 0xffe0, 16, true); //Marqueur APPO
    bitstream_write_nbits(jdesc->stream, 16, 16, false); //Longueur section
    bitstream_write_nbits(jdesc->stream, 0x4A46, 16, false); //JFIF\0
    bitstream_write_nbits(jdesc->stream, 0x494600, 24, false);
    bitstream_write_nbits(jdesc->stream, 1, 8, false);
    bitstream_write_nbits(jdesc->stream, 1, 8, false); //version 1->1
    bitstream_write_nbits(jdesc->stream, 0, 32, false);
    bitstream_write_nbits(jdesc->stream, 0, 24, false);

    //Define quantization table
    bitstream_write_nbits(jdesc->stream, 0xffdb, 16, true); //Marqueur DQT
    bitstream_write_nbits(jdesc->stream, 2+65*(1 + (int)(jdesc->nb_components > 1)), 16, false);
                                                            //Longueur section
    
    for (int table = 0; table < 1 + (int)(jdesc->nb_components > 1); table++){
        bitstream_write_nbits(jdesc->stream, 0, 4, false); //Précision table
        bitstream_write_nbits(jdesc->stream, table, 4, false); //indice table
        for (int i = 0; i < 64; i++){
            bitstream_write_nbits(jdesc->stream, jdesc->qtable[table][i], 8, false);
        }
    }

    //Start of frame
    bitstream_write_nbits(jdesc->stream, 0xffc0, 16, true); //Marqueur SOF0
    bitstream_write_nbits(jdesc->stream, 8 + 3*jdesc->nb_components, 16, false); //Longueur section
    bitstream_write_nbits(jdesc->stream, 8, 8, false);
    bitstream_write_nbits(jdesc->stream, jdesc->image_height, 16, false);//Hauteur image
    bitstream_write_nbits(jdesc->stream, jdesc->image_width, 16, false);//Largeur image
    bitstream_write_nbits(jdesc->stream, jdesc->nb_components, 8, false);//Nb composantes

    for (int component = 0; component < jdesc->nb_components; component++){
        bitstream_write_nbits(jdesc->stream, component, 8, false); // Identifiant de composante
        bitstream_write_nbits(jdesc->stream, jdesc->sampling_factor[component][0], 4, false); //H
        bitstream_write_nbits(jdesc->stream, jdesc->sampling_factor[component][1], 4, false); //V
        bitstream_write_nbits(jdesc->stream, (int)(component != 0), 8, false); 
        // Identifiant de composante
    }
    
    //Define Huffman table
    bitstream_write_nbits(jdesc->stream, 0xffc4, 16, true); //Marqueur DHT
    uint32_t nb_symbols = 0;

    for (int k = 0; k < 2 + 2*(int)(jdesc->nb_components > 1); k++){
        nb_symbols += htables_nb_symbols[k%2][k/2];
    }


    bitstream_write_nbits(jdesc->stream, 
            17*(1+(int)(jdesc->nb_components == 3))*2 + nb_symbols + 2 , 16, false); 
    //Longueur section 

    for (int k = 0; k < 2 + 2*(int)(jdesc->nb_components > 1); k++){

        int acdc = k%2;
        int component = k/2;
            bitstream_write_nbits(jdesc->stream, 0, 3, false); 
            bitstream_write_nbits(jdesc->stream, acdc, 1, false); 
            bitstream_write_nbits(jdesc->stream, component, 4, false); 
            
            uint8_t *table_length = huffman_table_get_length_vector(jdesc->htable[acdc][component]);
            //Tableau du nombre de symboles de chaque longueur associé à l'arbre de Huffman
            for (int i = 0; i < 16; i++){
                bitstream_write_nbits(jdesc->stream, table_length[i], 8, false);
            }

            uint8_t *table_huff = huffman_table_get_symbols(jdesc->htable[acdc][component]);
            //Tanle des symboles associés à l'arbre de Huffman
            uint8_t symbols = htables_nb_symbols[acdc][component];
            for (int j = 0; j < symbols; j++){
                bitstream_write_nbits(jdesc->stream, table_huff[j], 8, false);
            }
        
    }

    //Start of scan
    bitstream_write_nbits(jdesc->stream, 0xffda, 16, true); //Marqueur SOS
    bitstream_write_nbits(jdesc->stream, 2*jdesc->nb_components + 6, 16, false); //Longueur section
    bitstream_write_nbits(jdesc->stream, jdesc->nb_components, 8, false);

    bitstream_write_nbits(jdesc->stream, 0, 8, false); //Y 
    bitstream_write_nbits(jdesc->stream, 0, 4, false); 
    bitstream_write_nbits(jdesc->stream, 0, 4, false); 
    
    if (jdesc->nb_components == 3){
        bitstream_write_nbits(jdesc->stream, 1, 8, false); //Cb
        bitstream_write_nbits(jdesc->stream, 1, 4, false); 
        bitstream_write_nbits(jdesc->stream, 1, 4, false); 

        bitstream_write_nbits(jdesc->stream, 2, 8, false); //Cr
        bitstream_write_nbits(jdesc->stream, 1, 4, false); 
        bitstream_write_nbits(jdesc->stream, 1, 4, false); 
    }
    

    bitstream_write_nbits(jdesc->stream, 0, 8, false);
    bitstream_write_nbits(jdesc->stream, 63, 8, false);
    bitstream_write_nbits(jdesc->stream, 0, 8, false);

           
}


/* Ecrit le footer JPEG (marqueur EOI) dans le fichier de sortie. */
void jpeg_write_footer(struct jpeg_desc *jdesc){
    bitstream_write_nbits(jdesc->stream, 0xffd9, 16, true);
}

/*
    Détruit un jpeg_desc. Toute la mémoire qui lui est
    associée est libérée.
*/
void jpeg_desc_destroy(struct jpeg_desc *jdesc){
    bitstream_destroy(jdesc->stream);
    free(jdesc);
}


/*********
 *  GET  *
 *********/

            
/* Retourne le nom de fichier PPM lu dans le jpeg_desc jdesc. */
char *jpeg_desc_get_ppm_filename(struct jpeg_desc *jdesc){
    const char *ppm_filename = jdesc->ppm_filename;
    return (char *)ppm_filename;
}

/* Retourne le nom du fichier de sortie lu depuis le jpeg_desc jdesc. */
char *jpeg_desc_get_jpeg_filename(struct jpeg_desc *jdesc){
    const char *jpeg_filename = jdesc->jpeg_filename;
    return (char *)jpeg_filename;
}

/*
    Retourne la hauteur en nombre de pixels de l'image traitée lue dans le
    jpeg_desc jdesc.
*/
uint32_t jpeg_desc_get_image_height(struct jpeg_desc *jdesc){
    uint32_t height = jdesc->image_height;
    return height;
}

/*
    Retourne la largeur en nombre de pixels de l'image traitée lue dans le
    jpeg_desc jdesc.
*/
uint32_t jpeg_desc_egt_image_width(struct jpeg_desc *jdesc){
    uint32_t width = jdesc->image_width;
    return width;
}

/*
    Retourne le nombre de composantes de couleur de l'image traitée lu à partir
    du jpeg_desc jdesc.
*/
uint8_t jpeg_desc_get_nb_components(struct jpeg_desc *jdesc){
    uint8_t nb_components = jdesc->nb_components;
    return nb_components;
}

/*
    Retourne le bitstream associé au fichier de sortie enregistré dans le
    jpeg_desc.
*/
struct bitstream *jpeg_desc_get_bitstream(struct jpeg_desc *jdesc){
    struct bitstream *stream = jdesc->stream;
    return stream;
}

/*
    Retourne le facteur d'échantillonnage utilisé pour la composante de couleur
    cc et la direction dir, lu à partir du jpeg_desc jdesc.
*/
uint8_t jpeg_desc_get_sampling_factor(struct jpeg_desc *jdesc, enum color_component cc,
                                      enum direction dir){
    uint8_t sampling_factor = jdesc->sampling_factor[cc][dir];
    return sampling_factor;
}

/*
    Retourne un pointeur vers la table de Huffman utilisée pour encoder les
    données de la composante fréquentielle acdc pour la composante de couleur
    cc, lue à partir du jpeg_desc jdesc.
*/
struct huff_table *jpeg_desc_get_huffman_table(struct jpeg_desc *jdesc,
                                               enum sample_type acdc,
                                               enum color_component cc){
    struct huff_table *htable = jdesc->htable[acdc][cc];
    return htable;
}

/*
    Retourne la table de quantification associée à la composante de couleur
    cc dans le jpeg_desc jdesc.
*/
uint8_t *jpeg_desc_get_quantization_table(struct jpeg_desc *jdesc, enum color_component cc){ 
    uint8_t *qtable = jdesc->qtable[cc];
    return qtable;
}













