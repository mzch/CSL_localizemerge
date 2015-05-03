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
    char * id;         //
    char * index;      //
    char * key;        //
    char * value;
    char * translation;

} TranslatedTSV;

#define TAB     '\t'