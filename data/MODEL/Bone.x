xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 278;
 -9.48836;-0.06893;1.58000;,
 -9.48836;-1.18616;1.11723;,
 10.67165;-1.18615;1.11723;,
 10.67165;-0.06893;1.58000;,
 -9.48836;-1.64893;0.00000;,
 10.67165;-1.64892;-0.00000;,
 -9.48836;-1.18616;-1.11723;,
 10.67165;-1.18615;-1.11723;,
 -9.48836;-0.06893;-1.58000;,
 10.67165;-0.06893;-1.58000;,
 -9.48836;1.04830;-1.11723;,
 10.67165;1.04830;-1.11723;,
 -9.48836;1.51107;0.00000;,
 10.67165;1.51108;0.00000;,
 -9.48836;1.04830;1.11723;,
 10.67165;1.04830;1.11723;,
 -9.48836;-0.06893;1.58000;,
 10.67165;-0.06893;1.58000;,
 -9.48836;-0.06893;0.00000;,
 -9.48836;-0.06893;0.00000;,
 -9.48836;-0.06893;0.00000;,
 -9.48836;-0.06893;0.00000;,
 -9.48836;-0.06893;0.00000;,
 -9.48836;-0.06893;0.00000;,
 -9.48836;-0.06893;0.00000;,
 -9.48836;-0.06893;0.00000;,
 10.67165;-0.06893;0.00000;,
 10.67165;-0.06893;0.00000;,
 10.67165;-0.06893;0.00000;,
 10.67165;-0.06893;0.00000;,
 10.67165;-0.06893;0.00000;,
 10.67165;-0.06893;0.00000;,
 10.67165;-0.06893;0.00000;,
 10.67165;-0.06893;0.00000;,
 -13.04310;-1.33512;0.00000;,
 -12.77381;-2.04576;0.71064;,
 -12.77381;-1.33512;1.00500;,
 -13.04310;-1.33512;0.00000;,
 -12.77381;-2.34012;0.00000;,
 -13.04310;-1.33512;0.00000;,
 -12.77381;-2.04576;-0.71064;,
 -13.04310;-1.33512;0.00000;,
 -12.77381;-1.33512;-1.00500;,
 -13.04310;-1.33512;0.00000;,
 -12.77381;-0.62447;-0.71064;,
 -13.04310;-1.33512;0.00000;,
 -12.77381;-0.33012;0.00000;,
 -13.04310;-1.33512;0.00000;,
 -12.77381;-0.62447;0.71064;,
 -13.04310;-1.33512;0.00000;,
 -12.77381;-1.33512;1.00500;,
 -12.03810;-2.56598;1.23087;,
 -12.03810;-1.33512;1.74071;,
 -12.03810;-3.07583;0.00000;,
 -12.03810;-2.56598;-1.23087;,
 -12.03810;-1.33512;-1.74071;,
 -12.03810;-0.10425;-1.23087;,
 -12.03810;0.40560;0.00000;,
 -12.03810;-0.10425;1.23087;,
 -12.03810;-1.33512;1.74071;,
 -11.03310;-2.75640;1.42128;,
 -11.03310;-1.33512;2.01000;,
 -11.03310;-3.34511;0.00000;,
 -11.03310;-2.75640;-1.42128;,
 -11.03310;-1.33512;-2.01000;,
 -11.03310;0.08617;-1.42128;,
 -11.54858;0.40138;0.00000;,
 -11.03310;0.08617;1.42128;,
 -11.03310;-1.33512;2.01000;,
 -10.02810;-2.56598;1.23087;,
 -10.02810;-1.33512;1.74071;,
 -10.02810;-3.07583;0.00000;,
 -10.02810;-2.56598;-1.23087;,
 -10.02810;-1.33512;-1.74071;,
 -10.02810;-0.10425;-1.23087;,
 -10.02810;0.40559;0.00000;,
 -10.02810;-0.10425;1.23087;,
 -10.02810;-1.33512;1.74071;,
 -9.29239;-2.04576;0.71064;,
 -9.29239;-1.33512;1.00500;,
 -9.29239;-2.34012;0.00000;,
 -9.29239;-2.04576;-0.71064;,
 -9.29239;-1.33512;-1.00500;,
 -9.29239;-0.62447;-0.71064;,
 -9.29239;-0.33012;0.00000;,
 -9.29239;-0.62447;0.71064;,
 -9.29239;-1.33512;1.00500;,
 -9.02310;-1.33512;0.00000;,
 -9.02310;-1.33512;0.00000;,
 -9.02310;-1.33512;0.00000;,
 -9.02310;-1.33512;0.00000;,
 -9.02310;-1.33512;0.00000;,
 -9.02310;-1.33512;0.00000;,
 -9.02310;-1.33512;0.00000;,
 -9.02310;-1.33512;0.00000;,
 -13.01373;1.02489;0.00000;,
 -12.74444;0.31425;0.71064;,
 -12.74444;1.02489;1.00500;,
 -13.01373;1.02489;0.00000;,
 -12.74444;0.01989;0.00000;,
 -13.01373;1.02489;0.00000;,
 -12.74444;0.31425;-0.71064;,
 -13.01373;1.02489;0.00000;,
 -12.74444;1.02489;-1.00500;,
 -13.01373;1.02489;0.00000;,
 -12.74444;1.73554;-0.71064;,
 -13.01373;1.02489;0.00000;,
 -12.74444;2.02989;0.00000;,
 -13.01373;1.02489;0.00000;,
 -12.74444;1.73554;0.71064;,
 -13.01373;1.02489;0.00000;,
 -12.74444;1.02489;1.00500;,
 -12.00873;-0.20598;1.23087;,
 -12.00873;1.02489;1.74071;,
 -12.00873;-0.71582;0.00000;,
 -12.00873;-0.20598;-1.23087;,
 -12.00873;1.02489;-1.74071;,
 -12.00873;2.25576;-1.23087;,
 -12.00873;2.76560;0.00000;,
 -12.00873;2.25576;1.23087;,
 -12.00873;1.02489;1.74071;,
 -11.00373;-0.39639;1.42128;,
 -11.00373;1.02489;2.01000;,
 -11.00373;-0.98511;0.00000;,
 -11.00373;-0.39639;-1.42128;,
 -11.00373;1.02489;-2.01000;,
 -11.00373;2.44618;-1.42128;,
 -11.00373;3.03489;0.00000;,
 -11.00373;2.44618;1.42128;,
 -11.00373;1.02489;2.01000;,
 -9.99873;-0.20598;1.23087;,
 -9.99873;1.02489;1.74071;,
 -9.99873;-0.71582;0.00000;,
 -9.99873;-0.20598;-1.23087;,
 -9.99873;1.02489;-1.74071;,
 -9.99873;2.25576;-1.23087;,
 -9.99873;2.76560;0.00000;,
 -9.99873;2.25576;1.23087;,
 -9.99873;1.02489;1.74071;,
 -9.26302;0.31425;0.71064;,
 -9.26302;1.02489;1.00500;,
 -9.26302;0.01989;0.00000;,
 -9.26302;0.31425;-0.71064;,
 -9.26302;1.02489;-1.00500;,
 -9.26302;1.73553;-0.71064;,
 -9.26302;2.02989;0.00000;,
 -9.26302;1.73553;0.71064;,
 -9.26302;1.02489;1.00500;,
 -8.99373;1.02489;0.00000;,
 -8.99373;1.02489;0.00000;,
 -8.99373;1.02489;0.00000;,
 -8.99373;1.02489;0.00000;,
 -8.99373;1.02489;0.00000;,
 -8.99373;1.02489;0.00000;,
 -8.99373;1.02489;0.00000;,
 -8.99373;1.02489;0.00000;,
 8.67774;1.02490;0.00000;,
 8.94703;0.31426;0.71064;,
 8.94703;1.02490;1.00500;,
 8.67774;1.02490;0.00000;,
 8.94703;0.01990;0.00000;,
 8.67774;1.02490;0.00000;,
 8.94703;0.31426;-0.71064;,
 8.67774;1.02490;0.00000;,
 8.94703;1.02490;-1.00500;,
 8.67774;1.02490;0.00000;,
 8.94703;1.73554;-0.71064;,
 8.67774;1.02490;0.00000;,
 8.94703;2.02990;0.00000;,
 8.67774;1.02490;0.00000;,
 8.94703;1.73554;0.71064;,
 8.67774;1.02490;0.00000;,
 8.94703;1.02490;1.00500;,
 9.68274;-0.20597;1.23087;,
 9.68274;1.02490;1.74071;,
 9.68274;-0.71581;-0.00000;,
 9.68274;-0.20597;-1.23087;,
 9.68274;1.02490;-1.74071;,
 9.68274;2.25576;-1.23087;,
 9.68274;2.76561;0.00000;,
 9.68274;2.25576;1.23087;,
 9.68274;1.02490;1.74071;,
 10.68774;-0.39639;1.42128;,
 10.68774;1.02490;2.01000;,
 10.68774;-0.98510;-0.00000;,
 10.68774;-0.39639;-1.42128;,
 10.68774;1.02490;-2.01000;,
 10.68774;2.44618;-1.42128;,
 10.68775;3.03489;0.00000;,
 10.68774;2.44618;1.42128;,
 10.68774;1.02490;2.01000;,
 11.69274;-0.20597;1.23087;,
 11.69274;1.02489;1.74071;,
 11.69274;-0.71581;-0.00000;,
 11.69274;-0.20597;-1.23087;,
 11.69274;1.02489;-1.74071;,
 11.69274;2.25577;-1.23087;,
 11.69274;2.76561;0.00000;,
 11.69274;2.25577;1.23087;,
 11.69274;1.02489;1.74071;,
 12.42845;0.31425;0.71064;,
 12.42845;1.02490;1.00500;,
 12.42845;0.01990;0.00000;,
 12.42845;0.31425;-0.71064;,
 12.42845;1.02490;-1.00500;,
 12.42845;1.73554;-0.71064;,
 12.42845;2.02990;0.00000;,
 12.42845;1.73554;0.71064;,
 12.42845;1.02490;1.00500;,
 12.69774;1.02489;0.00000;,
 12.69774;1.02489;0.00000;,
 12.69774;1.02489;0.00000;,
 12.69774;1.02489;0.00000;,
 12.69774;1.02489;0.00000;,
 12.69774;1.02489;0.00000;,
 12.69774;1.02489;0.00000;,
 12.69774;1.02489;0.00000;,
 8.67774;-1.50096;0.00000;,
 8.94703;-2.21160;0.71064;,
 8.94703;-1.50096;1.00500;,
 8.67774;-1.50096;0.00000;,
 8.94703;-2.50596;-0.00000;,
 8.67774;-1.50096;0.00000;,
 8.94703;-2.21160;-0.71064;,
 8.67774;-1.50096;0.00000;,
 8.94703;-1.50096;-1.00500;,
 8.67774;-1.50096;0.00000;,
 8.94703;-0.79032;-0.71064;,
 8.67774;-1.50096;0.00000;,
 8.94703;-0.49596;0.00000;,
 8.67774;-1.50096;0.00000;,
 8.94703;-0.79032;0.71064;,
 8.67774;-1.50096;0.00000;,
 8.94703;-1.50096;1.00500;,
 9.68274;-2.73183;1.23087;,
 9.68274;-1.50096;1.74071;,
 9.68274;-3.24167;-0.00000;,
 9.68274;-2.73183;-1.23087;,
 9.68274;-1.50096;-1.74071;,
 9.68274;-0.27009;-1.23087;,
 9.68274;0.23975;0.00000;,
 9.68274;-0.27009;1.23087;,
 9.68274;-1.50096;1.74071;,
 10.68774;-2.92224;1.42128;,
 10.68774;-1.50096;2.01000;,
 10.68774;-3.51096;-0.00000;,
 10.68774;-2.92224;-1.42128;,
 10.68774;-1.50096;-2.01000;,
 10.68774;-0.07967;-1.42128;,
 10.68774;0.50904;0.00000;,
 10.68774;-0.07967;1.42128;,
 10.68774;-1.50096;2.01000;,
 11.69274;-2.73183;1.23087;,
 11.69274;-1.50096;1.74071;,
 11.69274;-3.24167;-0.00000;,
 11.69274;-2.73183;-1.23087;,
 11.69274;-1.50096;-1.74071;,
 11.69274;-0.27009;-1.23087;,
 11.69274;0.23975;0.00000;,
 11.69274;-0.27009;1.23087;,
 11.69274;-1.50096;1.74071;,
 12.42845;-2.21160;0.71064;,
 12.42845;-1.50096;1.00500;,
 12.42845;-2.50596;-0.00000;,
 12.42845;-2.21160;-0.71064;,
 12.42845;-1.50096;-1.00500;,
 12.42845;-0.79031;-0.71064;,
 12.42845;-0.49596;0.00000;,
 12.42845;-0.79032;0.71064;,
 12.42845;-1.50096;1.00500;,
 12.69774;-1.50096;0.00000;,
 12.69774;-1.50096;0.00000;,
 12.69774;-1.50096;0.00000;,
 12.69774;-1.50096;0.00000;,
 12.69774;-1.50096;0.00000;,
 12.69774;-1.50096;0.00000;,
 12.69774;-1.50096;0.00000;,
 12.69774;-1.50096;0.00000;;
 
 216;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 3;18,1,0;,
 3;19,4,1;,
 3;20,6,4;,
 3;21,8,6;,
 3;22,10,8;,
 3;23,12,10;,
 3;24,14,12;,
 3;25,16,14;,
 3;26,3,2;,
 3;27,2,5;,
 3;28,5,7;,
 3;29,7,9;,
 3;30,9,11;,
 3;31,11,13;,
 3;32,13,15;,
 3;33,15,17;,
 3;34,35,36;,
 3;37,38,35;,
 3;39,40,38;,
 3;41,42,40;,
 3;43,44,42;,
 3;45,46,44;,
 3;47,48,46;,
 3;49,50,48;,
 4;36,35,51,52;,
 4;35,38,53,51;,
 4;38,40,54,53;,
 4;40,42,55,54;,
 4;42,44,56,55;,
 4;44,46,57,56;,
 4;46,48,58,57;,
 4;48,50,59,58;,
 4;52,51,60,61;,
 4;51,53,62,60;,
 4;53,54,63,62;,
 4;54,55,64,63;,
 4;55,56,65,64;,
 4;56,57,66,65;,
 4;57,58,67,66;,
 4;58,59,68,67;,
 4;61,60,69,70;,
 4;60,62,71,69;,
 4;62,63,72,71;,
 4;63,64,73,72;,
 4;64,65,74,73;,
 4;65,66,75,74;,
 4;66,67,76,75;,
 4;67,68,77,76;,
 4;70,69,78,79;,
 4;69,71,80,78;,
 4;71,72,81,80;,
 4;72,73,82,81;,
 4;73,74,83,82;,
 4;74,75,84,83;,
 4;75,76,85,84;,
 4;76,77,86,85;,
 3;79,78,87;,
 3;78,80,88;,
 3;80,81,89;,
 3;81,82,90;,
 3;82,83,91;,
 3;83,84,92;,
 3;84,85,93;,
 3;85,86,94;,
 3;95,96,97;,
 3;98,99,96;,
 3;100,101,99;,
 3;102,103,101;,
 3;104,105,103;,
 3;106,107,105;,
 3;108,109,107;,
 3;110,111,109;,
 4;97,96,112,113;,
 4;96,99,114,112;,
 4;99,101,115,114;,
 4;101,103,116,115;,
 4;103,105,117,116;,
 4;105,107,118,117;,
 4;107,109,119,118;,
 4;109,111,120,119;,
 4;113,112,121,122;,
 4;112,114,123,121;,
 4;114,115,124,123;,
 4;115,116,125,124;,
 4;116,117,126,125;,
 4;117,118,127,126;,
 4;118,119,128,127;,
 4;119,120,129,128;,
 4;122,121,130,131;,
 4;121,123,132,130;,
 4;123,124,133,132;,
 4;124,125,134,133;,
 4;125,126,135,134;,
 4;126,127,136,135;,
 4;127,128,137,136;,
 4;128,129,138,137;,
 4;131,130,139,140;,
 4;130,132,141,139;,
 4;132,133,142,141;,
 4;133,134,143,142;,
 4;134,135,144,143;,
 4;135,136,145,144;,
 4;136,137,146,145;,
 4;137,138,147,146;,
 3;140,139,148;,
 3;139,141,149;,
 3;141,142,150;,
 3;142,143,151;,
 3;143,144,152;,
 3;144,145,153;,
 3;145,146,154;,
 3;146,147,155;,
 3;156,157,158;,
 3;159,160,157;,
 3;161,162,160;,
 3;163,164,162;,
 3;165,166,164;,
 3;167,168,166;,
 3;169,170,168;,
 3;171,172,170;,
 4;158,157,173,174;,
 4;157,160,175,173;,
 4;160,162,176,175;,
 4;162,164,177,176;,
 4;164,166,178,177;,
 4;166,168,179,178;,
 4;168,170,180,179;,
 4;170,172,181,180;,
 4;174,173,182,183;,
 4;173,175,184,182;,
 4;175,176,185,184;,
 4;176,177,186,185;,
 4;177,178,187,186;,
 4;178,179,188,187;,
 4;179,180,189,188;,
 4;180,181,190,189;,
 4;183,182,191,192;,
 4;182,184,193,191;,
 4;184,185,194,193;,
 4;185,186,195,194;,
 4;186,187,196,195;,
 4;187,188,197,196;,
 4;188,189,198,197;,
 4;189,190,199,198;,
 4;192,191,200,201;,
 4;191,193,202,200;,
 4;193,194,203,202;,
 4;194,195,204,203;,
 4;195,196,205,204;,
 4;196,197,206,205;,
 4;197,198,207,206;,
 4;198,199,208,207;,
 3;201,200,209;,
 3;200,202,210;,
 3;202,203,211;,
 3;203,204,212;,
 3;204,205,213;,
 3;205,206,214;,
 3;206,207,215;,
 3;207,208,216;,
 3;217,218,219;,
 3;220,221,218;,
 3;222,223,221;,
 3;224,225,223;,
 3;226,227,225;,
 3;228,229,227;,
 3;230,231,229;,
 3;232,233,231;,
 4;219,218,234,235;,
 4;218,221,236,234;,
 4;221,223,237,236;,
 4;223,225,238,237;,
 4;225,227,239,238;,
 4;227,229,240,239;,
 4;229,231,241,240;,
 4;231,233,242,241;,
 4;235,234,243,244;,
 4;234,236,245,243;,
 4;236,237,246,245;,
 4;237,238,247,246;,
 4;238,239,248,247;,
 4;239,240,249,248;,
 4;240,241,250,249;,
 4;241,242,251,250;,
 4;244,243,252,253;,
 4;243,245,254,252;,
 4;245,246,255,254;,
 4;246,247,256,255;,
 4;247,248,257,256;,
 4;248,249,258,257;,
 4;249,250,259,258;,
 4;250,251,260,259;,
 4;253,252,261,262;,
 4;252,254,263,261;,
 4;254,255,264,263;,
 4;255,256,265,264;,
 4;256,257,266,265;,
 4;257,258,267,266;,
 4;258,259,268,267;,
 4;259,260,269,268;,
 3;262,261,270;,
 3;261,263,271;,
 3;263,264,272;,
 3;264,265,273;,
 3;265,266,274;,
 3;266,267,275;,
 3;267,268,276;,
 3;268,269,277;;
 
 MeshMaterialList {
  2;
  216;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  178;
  -1.000000;0.000000;0.000000;,
  0.000000;-0.000001;1.000000;,
  0.000000;-0.707107;0.707106;,
  0.000000;-1.000000;0.000000;,
  0.000000;-0.707107;-0.707106;,
  0.000000;-0.000001;-1.000000;,
  -0.000000;0.707105;-0.707108;,
  -0.000000;1.000000;0.000000;,
  -0.000000;0.707105;0.707108;,
  1.000000;0.000000;0.000000;,
  -1.000000;-0.000000;-0.000000;,
  -0.868389;-0.000001;0.495883;,
  -0.868388;-0.350643;0.350644;,
  -0.868389;-0.495884;0.000000;,
  -0.868388;-0.350643;-0.350643;,
  -0.868389;-0.000001;-0.495883;,
  -0.868389;0.350642;-0.350642;,
  -0.868389;0.495883;0.000000;,
  -0.868389;0.350642;0.350642;,
  -0.503834;-0.000001;0.863801;,
  -0.503833;-0.610800;0.610800;,
  -0.503833;-0.863801;0.000000;,
  -0.503833;-0.610800;-0.610800;,
  -0.503834;-0.000001;-0.863801;,
  -0.476532;0.631296;-0.611869;,
  -0.440369;0.897817;0.000000;,
  -0.476532;0.631296;0.611869;,
  -0.000000;-0.000001;1.000000;,
  -0.000000;-0.707107;0.707106;,
  -0.000000;-1.000000;0.000000;,
  -0.000000;-0.707107;-0.707106;,
  -0.000000;-0.000001;-1.000000;,
  0.002169;0.733653;-0.679520;,
  0.004124;0.999992;0.000000;,
  0.002169;0.733653;0.679520;,
  0.503834;-0.000001;0.863801;,
  0.503832;-0.610800;0.610800;,
  0.503832;-0.863802;0.000000;,
  0.503832;-0.610800;-0.610800;,
  0.503834;-0.000001;-0.863801;,
  0.480444;0.637238;-0.602579;,
  0.440984;0.897515;0.000000;,
  0.480444;0.637238;0.602579;,
  0.868389;-0.000001;0.495883;,
  0.868389;-0.350643;0.350643;,
  0.868389;-0.495883;0.000000;,
  0.868389;-0.350643;-0.350643;,
  0.868389;-0.000001;-0.495883;,
  0.868389;0.350643;-0.350641;,
  0.868388;0.495885;0.000000;,
  0.868389;0.350643;0.350641;,
  1.000000;-0.000000;0.000000;,
  -1.000000;-0.000000;0.000000;,
  -0.868389;-0.000001;0.495883;,
  -0.868389;-0.350642;0.350642;,
  -0.868389;-0.495883;0.000000;,
  -0.868389;-0.350642;-0.350642;,
  -0.868389;-0.000001;-0.495883;,
  -0.868389;0.350643;-0.350641;,
  -0.868388;0.495885;0.000000;,
  -0.868389;0.350643;0.350641;,
  -0.503834;-0.000001;0.863801;,
  -0.503833;-0.610799;0.610799;,
  -0.503834;-0.863801;0.000000;,
  -0.503833;-0.610799;-0.610799;,
  -0.503834;-0.000001;-0.863801;,
  -0.503834;0.610800;-0.610798;,
  -0.503833;0.863801;0.000000;,
  -0.503834;0.610800;0.610798;,
  0.000000;-0.000001;1.000000;,
  0.000000;-0.707107;0.707107;,
  0.000000;-1.000000;0.000000;,
  0.000000;-0.707107;-0.707107;,
  -0.000000;-0.000001;-1.000000;,
  0.000000;0.707107;-0.707106;,
  0.000000;1.000000;0.000000;,
  0.000000;0.707107;0.707106;,
  0.503834;-0.000000;0.863800;,
  0.503833;-0.610799;0.610799;,
  0.503834;-0.863801;0.000000;,
  0.503833;-0.610799;-0.610799;,
  0.503834;-0.000000;-0.863800;,
  0.503835;0.610799;-0.610798;,
  0.503836;0.863799;0.000000;,
  0.503835;0.610799;0.610798;,
  0.868390;0.000000;0.495883;,
  0.868389;-0.350642;0.350642;,
  0.868390;-0.495883;0.000000;,
  0.868389;-0.350642;-0.350642;,
  0.868390;0.000000;-0.495883;,
  0.868389;0.350642;-0.350642;,
  0.868390;0.495882;0.000000;,
  0.868389;0.350642;0.350642;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;-0.000000;,
  -0.868389;0.000000;0.495883;,
  -0.868389;-0.350643;0.350643;,
  -0.868389;-0.495883;0.000000;,
  -0.868389;-0.350643;-0.350643;,
  -0.868389;0.000000;-0.495883;,
  -0.868388;0.350643;-0.350644;,
  -0.868389;0.495884;0.000000;,
  -0.868389;0.350643;0.350643;,
  -0.503834;0.000001;0.863800;,
  -0.503835;-0.610799;0.610799;,
  -0.503835;-0.863800;0.000000;,
  -0.503835;-0.610799;-0.610799;,
  -0.503834;0.000001;-0.863800;,
  -0.503832;0.610800;-0.610800;,
  -0.503832;0.863802;0.000000;,
  -0.503832;0.610800;0.610800;,
  -0.000000;0.000000;1.000000;,
  -0.000000;-0.707108;0.707106;,
  0.000000;-1.000000;0.000000;,
  -0.000000;-0.707108;-0.707106;,
  -0.000000;0.000000;-1.000000;,
  -0.000001;0.707107;-0.707106;,
  -0.000001;1.000000;0.000000;,
  -0.000001;0.707107;0.707106;,
  0.503834;-0.000000;0.863801;,
  0.503834;-0.610801;0.610798;,
  0.503833;-0.863801;0.000000;,
  0.503834;-0.610801;-0.610798;,
  0.503834;-0.000000;-0.863801;,
  0.503833;0.610799;-0.610799;,
  0.503833;0.863801;0.000000;,
  0.503833;0.610799;0.610799;,
  0.868389;0.000000;0.495883;,
  0.868389;-0.350644;0.350641;,
  0.868388;-0.495886;0.000000;,
  0.868389;-0.350644;-0.350641;,
  0.868389;0.000000;-0.495883;,
  0.868390;0.350641;-0.350642;,
  0.868390;0.495881;0.000000;,
  0.868390;0.350641;0.350642;,
  1.000000;-0.000001;0.000000;,
  -1.000000;0.000000;0.000000;,
  -0.868390;0.000000;0.495883;,
  -0.868389;-0.350642;0.350642;,
  -0.868389;-0.495883;0.000000;,
  -0.868389;-0.350642;-0.350642;,
  -0.868390;0.000000;-0.495883;,
  -0.868389;0.350643;-0.350643;,
  -0.868389;0.495883;0.000000;,
  -0.868389;0.350643;0.350643;,
  -0.503834;-0.000000;0.863800;,
  -0.503834;-0.610799;0.610799;,
  -0.503834;-0.863801;0.000000;,
  -0.503834;-0.610799;-0.610799;,
  -0.503834;-0.000000;-0.863800;,
  -0.503835;0.610799;-0.610799;,
  -0.503835;0.863800;0.000000;,
  -0.503835;0.610799;0.610799;,
  0.000000;-0.000001;1.000000;,
  0.000000;-0.707107;0.707107;,
  0.000000;-1.000000;0.000000;,
  0.000000;-0.707107;-0.707107;,
  0.000000;-0.000001;-1.000000;,
  0.000000;0.707107;-0.707106;,
  0.000000;1.000000;0.000000;,
  0.000000;0.707107;0.707106;,
  0.503834;-0.000000;0.863800;,
  0.503834;-0.610799;0.610799;,
  0.503834;-0.863801;0.000000;,
  0.503834;-0.610799;-0.610799;,
  0.503834;-0.000001;-0.863800;,
  0.503834;0.610800;-0.610798;,
  0.503834;0.863800;0.000001;,
  0.503835;0.610799;0.610798;,
  0.868390;0.000000;0.495883;,
  0.868389;-0.350642;0.350642;,
  0.868389;-0.495883;0.000000;,
  0.868389;-0.350642;-0.350642;,
  0.868389;-0.000001;-0.495883;,
  0.868389;0.350643;-0.350641;,
  0.868389;0.495884;0.000002;,
  0.868389;0.350643;0.350643;,
  1.000000;-0.000000;0.000000;;
  216;
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,1,1,8;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;10,12,11;,
  3;10,13,12;,
  3;10,14,13;,
  3;10,15,14;,
  3;10,16,15;,
  3;10,17,16;,
  3;10,18,17;,
  3;10,11,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,16,24,23;,
  4;16,17,25,24;,
  4;17,18,26,25;,
  4;18,11,19,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,24,32,31;,
  4;24,25,33,32;,
  4;25,26,34,33;,
  4;26,19,27,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,32,40,39;,
  4;32,33,41,40;,
  4;33,34,42,41;,
  4;34,27,35,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,40,48,47;,
  4;40,41,49,48;,
  4;41,42,50,49;,
  4;42,35,43,50;,
  3;43,44,51;,
  3;44,45,51;,
  3;45,46,51;,
  3;46,47,51;,
  3;47,48,51;,
  3;48,49,51;,
  3;49,50,51;,
  3;50,43,51;,
  3;52,54,53;,
  3;52,55,54;,
  3;52,56,55;,
  3;52,57,56;,
  3;52,58,57;,
  3;52,59,58;,
  3;52,60,59;,
  3;52,53,60;,
  4;53,54,62,61;,
  4;54,55,63,62;,
  4;55,56,64,63;,
  4;56,57,65,64;,
  4;57,58,66,65;,
  4;58,59,67,66;,
  4;59,60,68,67;,
  4;60,53,61,68;,
  4;61,62,70,69;,
  4;62,63,71,70;,
  4;63,64,72,71;,
  4;64,65,73,72;,
  4;65,66,74,73;,
  4;66,67,75,74;,
  4;67,68,76,75;,
  4;68,61,69,76;,
  4;69,70,78,77;,
  4;70,71,79,78;,
  4;71,72,80,79;,
  4;72,73,81,80;,
  4;73,74,82,81;,
  4;74,75,83,82;,
  4;75,76,84,83;,
  4;76,69,77,84;,
  4;77,78,86,85;,
  4;78,79,87,86;,
  4;79,80,88,87;,
  4;80,81,89,88;,
  4;81,82,90,89;,
  4;82,83,91,90;,
  4;83,84,92,91;,
  4;84,77,85,92;,
  3;85,86,93;,
  3;86,87,93;,
  3;87,88,93;,
  3;88,89,93;,
  3;89,90,93;,
  3;90,91,93;,
  3;91,92,93;,
  3;92,85,93;,
  3;94,96,95;,
  3;94,97,96;,
  3;94,98,97;,
  3;94,99,98;,
  3;94,100,99;,
  3;94,101,100;,
  3;94,102,101;,
  3;94,95,102;,
  4;95,96,104,103;,
  4;96,97,105,104;,
  4;97,98,106,105;,
  4;98,99,107,106;,
  4;99,100,108,107;,
  4;100,101,109,108;,
  4;101,102,110,109;,
  4;102,95,103,110;,
  4;103,104,112,111;,
  4;104,105,113,112;,
  4;105,106,114,113;,
  4;106,107,115,114;,
  4;107,108,116,115;,
  4;108,109,117,116;,
  4;109,110,118,117;,
  4;110,103,111,118;,
  4;111,112,120,119;,
  4;112,113,121,120;,
  4;113,114,122,121;,
  4;114,115,123,122;,
  4;115,116,124,123;,
  4;116,117,125,124;,
  4;117,118,126,125;,
  4;118,111,119,126;,
  4;119,120,128,127;,
  4;120,121,129,128;,
  4;121,122,130,129;,
  4;122,123,131,130;,
  4;123,124,132,131;,
  4;124,125,133,132;,
  4;125,126,134,133;,
  4;126,119,127,134;,
  3;127,128,135;,
  3;128,129,135;,
  3;129,130,135;,
  3;130,131,135;,
  3;131,132,135;,
  3;132,133,135;,
  3;133,134,135;,
  3;134,127,135;,
  3;136,138,137;,
  3;136,139,138;,
  3;136,140,139;,
  3;136,141,140;,
  3;136,142,141;,
  3;136,143,142;,
  3;136,144,143;,
  3;136,137,144;,
  4;137,138,146,145;,
  4;138,139,147,146;,
  4;139,140,148,147;,
  4;140,141,149,148;,
  4;141,142,150,149;,
  4;142,143,151,150;,
  4;143,144,152,151;,
  4;144,137,145,152;,
  4;145,146,154,153;,
  4;146,147,155,154;,
  4;147,148,156,155;,
  4;148,149,157,156;,
  4;149,150,158,157;,
  4;150,151,159,158;,
  4;151,152,160,159;,
  4;152,145,153,160;,
  4;153,154,162,161;,
  4;154,155,163,162;,
  4;155,156,164,163;,
  4;156,157,165,164;,
  4;157,158,166,165;,
  4;158,159,167,166;,
  4;159,160,168,167;,
  4;160,153,161,168;,
  4;161,162,170,169;,
  4;162,163,171,170;,
  4;163,164,172,171;,
  4;164,165,173,172;,
  4;165,166,174,173;,
  4;166,167,175,174;,
  4;167,168,176,175;,
  4;168,161,169,176;,
  3;169,170,177;,
  3;170,171,177;,
  3;171,172,177;,
  3;172,173,177;,
  3;173,174,177;,
  3;174,175,177;,
  3;175,176,177;,
  3;176,169,177;;
 }
 MeshTextureCoords {
  278;
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.062500;0.000000;,
  0.125000;0.166670;,
  0.000000;0.166670;,
  0.187500;0.000000;,
  0.250000;0.166670;,
  0.312500;0.000000;,
  0.375000;0.166670;,
  0.437500;0.000000;,
  0.500000;0.166670;,
  0.562500;0.000000;,
  0.625000;0.166670;,
  0.687500;0.000000;,
  0.750000;0.166670;,
  0.812500;0.000000;,
  0.875000;0.166670;,
  0.937500;0.000000;,
  1.000000;0.166670;,
  0.125000;0.333330;,
  0.000000;0.333330;,
  0.250000;0.333330;,
  0.375000;0.333330;,
  0.500000;0.333330;,
  0.625000;0.333330;,
  0.750000;0.333330;,
  0.875000;0.333330;,
  1.000000;0.333330;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.666670;,
  0.000000;0.666670;,
  0.250000;0.666670;,
  0.375000;0.666670;,
  0.500000;0.666670;,
  0.625000;0.666670;,
  0.750000;0.666670;,
  0.875000;0.666670;,
  1.000000;0.666670;,
  0.125000;0.833330;,
  0.000000;0.833330;,
  0.250000;0.833330;,
  0.375000;0.833330;,
  0.500000;0.833330;,
  0.625000;0.833330;,
  0.750000;0.833330;,
  0.875000;0.833330;,
  1.000000;0.833330;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.062500;0.000000;,
  0.125000;0.166670;,
  0.000000;0.166670;,
  0.187500;0.000000;,
  0.250000;0.166670;,
  0.312500;0.000000;,
  0.375000;0.166670;,
  0.437500;0.000000;,
  0.500000;0.166670;,
  0.562500;0.000000;,
  0.625000;0.166670;,
  0.687500;0.000000;,
  0.750000;0.166670;,
  0.812500;0.000000;,
  0.875000;0.166670;,
  0.937500;0.000000;,
  1.000000;0.166670;,
  0.125000;0.333330;,
  0.000000;0.333330;,
  0.250000;0.333330;,
  0.375000;0.333330;,
  0.500000;0.333330;,
  0.625000;0.333330;,
  0.750000;0.333330;,
  0.875000;0.333330;,
  1.000000;0.333330;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.666670;,
  0.000000;0.666670;,
  0.250000;0.666670;,
  0.375000;0.666670;,
  0.500000;0.666670;,
  0.625000;0.666670;,
  0.750000;0.666670;,
  0.875000;0.666670;,
  1.000000;0.666670;,
  0.125000;0.833330;,
  0.000000;0.833330;,
  0.250000;0.833330;,
  0.375000;0.833330;,
  0.500000;0.833330;,
  0.625000;0.833330;,
  0.750000;0.833330;,
  0.875000;0.833330;,
  1.000000;0.833330;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.062500;0.000000;,
  0.125000;0.166670;,
  0.000000;0.166670;,
  0.187500;0.000000;,
  0.250000;0.166670;,
  0.312500;0.000000;,
  0.375000;0.166670;,
  0.437500;0.000000;,
  0.500000;0.166670;,
  0.562500;0.000000;,
  0.625000;0.166670;,
  0.687500;0.000000;,
  0.750000;0.166670;,
  0.812500;0.000000;,
  0.875000;0.166670;,
  0.937500;0.000000;,
  1.000000;0.166670;,
  0.125000;0.333330;,
  0.000000;0.333330;,
  0.250000;0.333330;,
  0.375000;0.333330;,
  0.500000;0.333330;,
  0.625000;0.333330;,
  0.750000;0.333330;,
  0.875000;0.333330;,
  1.000000;0.333330;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.666670;,
  0.000000;0.666670;,
  0.250000;0.666670;,
  0.375000;0.666670;,
  0.500000;0.666670;,
  0.625000;0.666670;,
  0.750000;0.666670;,
  0.875000;0.666670;,
  1.000000;0.666670;,
  0.125000;0.833330;,
  0.000000;0.833330;,
  0.250000;0.833330;,
  0.375000;0.833330;,
  0.500000;0.833330;,
  0.625000;0.833330;,
  0.750000;0.833330;,
  0.875000;0.833330;,
  1.000000;0.833330;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.062500;0.000000;,
  0.125000;0.166670;,
  0.000000;0.166670;,
  0.187500;0.000000;,
  0.250000;0.166670;,
  0.312500;0.000000;,
  0.375000;0.166670;,
  0.437500;0.000000;,
  0.500000;0.166670;,
  0.562500;0.000000;,
  0.625000;0.166670;,
  0.687500;0.000000;,
  0.750000;0.166670;,
  0.812500;0.000000;,
  0.875000;0.166670;,
  0.937500;0.000000;,
  1.000000;0.166670;,
  0.125000;0.333330;,
  0.000000;0.333330;,
  0.250000;0.333330;,
  0.375000;0.333330;,
  0.500000;0.333330;,
  0.625000;0.333330;,
  0.750000;0.333330;,
  0.875000;0.333330;,
  1.000000;0.333330;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.666670;,
  0.000000;0.666670;,
  0.250000;0.666670;,
  0.375000;0.666670;,
  0.500000;0.666670;,
  0.625000;0.666670;,
  0.750000;0.666670;,
  0.875000;0.666670;,
  1.000000;0.666670;,
  0.125000;0.833330;,
  0.000000;0.833330;,
  0.250000;0.833330;,
  0.375000;0.833330;,
  0.500000;0.833330;,
  0.625000;0.833330;,
  0.750000;0.833330;,
  0.875000;0.833330;,
  1.000000;0.833330;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;;
 }
}
