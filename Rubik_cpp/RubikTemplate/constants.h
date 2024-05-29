/**
    @author Robert Kirk DeLisle rkdelisle@gmail.com
    @version 0.1

    @section LICENSE

    @section DESCRIPTION

    Constants used by the 2x2x2 Rubik's cube class, Cube2
*/


#if !defined(RubikConstants_07Jun19)
#define RubikConstants_07Jun19

#include <map>
#include <string>

///constants for the faces/rotational planes
///To store rotational plane information, binary encoded as:  DURLBF
enum RubikFaces
{
    FRONT = 1,
    UP = 16,
    LEFT = 4,
    RIGHT = 8,
    DOWN = 32,
    BACK = 2
};

///constants for the colors - these were taken from my 2x2x2 cube but should probably be more dynamic
enum RubikColors
{
    RED,
    BLUE,
    YELLOW,
    GREEN,
    WHITE,
    ORANGE
};

///combined with face constants can be used to store the type of move
enum RubikFaceRotations
{
    CLOCK,
    COUNTER,
    ONE80
};

///these are the corner cubies in any sized cube
///To store rotational plane information, binary encoded as:  DURLBF
enum CornerCubies
{
    DLF = 37, ///this one is used for the standard reference and orientation in 2x2x2
    DRF = 41,
    DLB = 38,
    DRB = 42,
    ULF = 21,
    URF = 25,
    ULB = 22,
    URB = 26
};

std::string CornerToString(CornerCubies cc)
{
    switch (cc)
    {
        case DLF:
            return "DLF";
            break;
        case DRF:
            return "DRF";
            break;
        case DRB:
            return "DRB";
            break;
        case DLB:
            return "DLB";
            break;
        case ULF:
            return "ULF";
            break;
        case URF:
            return "URF";
            break;
        case ULB:
            return "ULB";
            break;
        case URB:
            return "URB";
            break;
        default:
            return "";
    }
};

//std::map<CornerCubies, std::string> CornerToString;
//CornerToString[DLF] = "DLF";
//CornerToString[DRF] = "DRF";
//CornerToString[DRB] = "DRB";
//CornerToString[DLB] = "DLB";
//CornerToString[ULF] = "ULF";
//CornerToString[URF] = "URF";
//CornerToString[ULB] = "ULB";
//CornerToString[URB] = "URB";

///references for edge cubies in 3x3x3 cube
///To store rotational plane information, binary encoded as:  DURLBF
enum EdgeCubies3
{
    DL = 36,  //"DOWN LEFT"
    DF = 33,  //"DOWN FRONT"
    DR = 40,  //"DOWN RIGHT"
    DB = 34,  //"DOWN BACK"
    LF = 5,  //"LEFT FRONT"
    RF = 9,  //"RIGHT FRONT"
    RB = 10,  //"RIGHT BACK"
    LB = 6,  //"LEFT BACK"
    UL = 14,  //"UP LEFT"
    UF = 11,  //"UP FRONT"
    UR = 18,  //"UP RIGHT"
    UB = 12  //"UP BACK"
};

std::string Edge3ToString(EdgeCubies3 ec)
{
    switch (ec):
    {
        case DL:
            return "DL";
            break;
        case DF:
            return "DF";
            break;
        case DR:
            return "DR";
            break;
        case DB:
            return "DB";
            break;
        case LF:
            return "LF";
            break;
        case RF:
            return "RF";
            break;
        case LB:
            return "LB";
            break;
        case RB:
            return "RB";
            break;
        case UL:
            return "UL";
            break;
        case UF:
            return "UF";
            break;
        case UR:
            return "UR";
            break;
        case UB:
            return "UB";
            break;
        default:
            return "";
    }
};
//std::map<EdgeCubies3, std::string> Edge3ToString;
//Edge3ToString[DL] = "DL";
//Edge3ToString[DF] = "DF";
//Edge3ToString[DR] = "DR";
//Edge3ToString[DB] = "DB";
//Edge3ToString[LF] = "LF";
//Edge3ToString[RF] = "RF";
//Edge3ToString[RB] = "RB";
//Edge3ToString[LB] = "LB";
//Edge3ToString[UL] = "DL";
//Edge3ToString[UF] = "DL";
//Edge3ToString[UR] = "DL";
//Edge3ToString[UB] = "DL";

#endif
