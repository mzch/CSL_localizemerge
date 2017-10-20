//
//

typedef struct {
    char * name;
    char * key;
    char * ref_name;   //
    char * ref_key;    //
    char * index;      //
    char * value;
    char * translation;
} LocaleTSV;

typedef struct {
    char * line_num;    // line number
    char * id;          //
    char * index;       //
    char * key;         //
    char * value;
    char * translation;

} TranslatedTSV;

#define TAB         '\t'
#define SPC         ' '
#define CRLF        '\n'
#define BUFFER_SIZE (8192 * 4)
