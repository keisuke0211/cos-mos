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
 155;
 -1.13883;-0.86098;0.00000;,
 -1.47864;-0.92773;0.23729;,
 -1.13883;-0.92773;0.33558;,
 -1.13883;-0.86098;0.00000;,
 -1.61940;-0.92773;0.00000;,
 -1.13883;-0.86098;0.00000;,
 -1.47864;-0.92773;-0.23729;,
 -1.13883;-0.86098;0.00000;,
 -1.13883;-0.92773;-0.33558;,
 -1.13883;-0.86098;0.00000;,
 -0.79900;-0.92773;-0.23729;,
 -1.13883;-0.86098;0.00000;,
 -0.65825;-0.92773;0.00000;,
 -1.13883;-0.86098;0.00000;,
 -0.79900;-0.92773;0.23729;,
 -1.13883;-0.86098;0.00000;,
 -1.13883;-0.92773;0.33558;,
 -1.76672;-1.11783;0.43846;,
 -1.13883;-1.11783;0.62008;,
 -2.02680;-1.11783;0.00000;,
 -1.76672;-1.11783;-0.43846;,
 -1.13883;-1.11783;-0.62008;,
 -0.51092;-1.11783;-0.43846;,
 -0.25084;-1.11783;0.00000;,
 -0.51092;-1.11783;0.43846;,
 -1.13883;-1.11783;0.62008;,
 -1.95921;-1.40232;0.57288;,
 -1.13883;-1.40232;0.81017;,
 -2.29903;-1.40232;0.00000;,
 -1.95921;-1.40232;-0.57288;,
 -1.13883;-1.40232;-0.81017;,
 -0.31844;-1.40232;-0.57288;,
 0.02137;-1.40232;0.00000;,
 -0.31844;-1.40232;0.57288;,
 -1.13883;-1.40232;0.81017;,
 -2.02680;-1.73791;0.62008;,
 -1.13883;-1.73791;0.87692;,
 -2.39461;-1.73791;0.00000;,
 -2.02680;-1.73791;-0.62008;,
 -1.13883;-1.73791;-0.87692;,
 -0.25084;-1.73791;-0.62008;,
 0.11697;-1.73791;0.00000;,
 -0.25084;-1.73791;0.62008;,
 -1.13883;-1.73791;0.87692;,
 -1.95921;-2.07349;0.57288;,
 -1.13883;-2.07349;0.81017;,
 -2.29903;-2.07349;0.00000;,
 -1.95921;-2.07349;-0.57288;,
 -1.13883;-2.07349;-0.81017;,
 -0.31844;-2.07349;-0.57288;,
 0.02137;-2.07349;0.00000;,
 -0.31844;-2.07349;0.57288;,
 -1.13883;-2.07349;0.81017;,
 -1.76672;-2.35799;0.43846;,
 -1.13883;-2.35799;0.62008;,
 -2.02680;-2.35799;0.00000;,
 -1.76672;-2.35799;-0.43846;,
 -1.13883;-2.35799;-0.62008;,
 -0.51092;-2.35799;-0.43846;,
 -0.25084;-2.35799;0.00000;,
 -0.51092;-2.35799;0.43846;,
 -1.13883;-2.35799;0.62008;,
 -1.47864;-2.54808;0.23729;,
 -1.13883;-2.54808;0.33558;,
 -1.61940;-2.54808;0.00000;,
 -1.47864;-2.54808;-0.23729;,
 -1.13883;-2.54808;-0.33558;,
 -0.79900;-2.54808;-0.23729;,
 -0.65825;-2.54808;0.00000;,
 -0.79900;-2.54808;0.23729;,
 -1.13883;-2.54808;0.33558;,
 -1.13883;-2.61483;0.00000;,
 -1.13883;-2.61483;0.00000;,
 -1.13883;-2.61483;0.00000;,
 -1.13883;-2.61483;0.00000;,
 -1.13883;-2.61483;0.00000;,
 -1.13883;-2.61483;0.00000;,
 -1.13883;-2.61483;0.00000;,
 -1.13883;-2.61483;0.00000;,
 -0.49943;2.62038;0.33490;,
 -0.73623;2.62038;0.23681;,
 -0.73623;-1.52693;0.23681;,
 -0.49943;-1.52693;0.33490;,
 -0.83432;2.62038;0.00000;,
 -0.83432;-1.52693;0.00000;,
 -0.73623;2.62038;-0.23681;,
 -0.73623;-1.52693;-0.23681;,
 -0.49943;2.62038;-0.33490;,
 -0.49943;-1.52693;-0.33490;,
 -0.26262;2.62038;-0.23681;,
 -0.26262;-1.52693;-0.23681;,
 -0.16453;2.62038;0.00000;,
 -0.16453;-1.52693;0.00000;,
 -0.26262;2.62038;0.23681;,
 -0.26262;-1.52693;0.23681;,
 -0.49943;2.62038;0.33490;,
 -0.49943;-1.52693;0.33490;,
 -0.49943;2.62038;0.00000;,
 -0.49943;2.62038;0.00000;,
 -0.49943;2.62038;0.00000;,
 -0.49943;2.62038;0.00000;,
 -0.49943;2.62038;0.00000;,
 -0.49943;2.62038;0.00000;,
 -0.49943;2.62038;0.00000;,
 -0.49943;2.62038;0.00000;,
 -0.49943;-1.52693;0.00000;,
 -0.49943;-1.52693;0.00000;,
 -0.49943;-1.52693;0.00000;,
 -0.49943;-1.52693;0.00000;,
 -0.49943;-1.52693;0.00000;,
 -0.49943;-1.52693;0.00000;,
 -0.49943;-1.52693;0.00000;,
 -0.49943;-1.52693;0.00000;,
 2.05530;1.32453;0.07030;,
 1.72697;1.79644;0.13715;,
 1.72697;1.22214;0.13715;,
 2.05530;1.03014;0.07030;,
 1.24321;2.15665;0.18043;,
 1.24321;1.40107;0.18043;,
 0.60994;2.41052;0.22248;,
 0.60994;1.47888;0.22248;,
 -0.13758;2.61016;0.25839;,
 -0.13758;1.52817;0.25839;,
 -0.13758;2.61016;0.25839;,
 -0.13758;2.61016;-0.25839;,
 -0.13758;1.52817;-0.25839;,
 -0.13758;1.52817;0.25839;,
 -0.13758;2.61016;-0.25839;,
 0.60994;2.41052;-0.22248;,
 0.60994;1.47888;-0.22248;,
 -0.13758;1.52817;-0.25839;,
 1.24321;2.15665;-0.18043;,
 1.24321;1.40107;-0.18043;,
 1.72697;1.79644;-0.13715;,
 1.72697;1.22214;-0.13715;,
 2.05530;1.32453;-0.07030;,
 2.05530;1.03014;-0.07030;,
 2.05530;1.32453;-0.07030;,
 2.05530;1.32453;0.07030;,
 2.05530;1.03014;0.07030;,
 2.05530;1.03014;-0.07030;,
 1.72697;1.79644;-0.13715;,
 1.72697;1.79644;0.13715;,
 2.05530;1.32453;0.07030;,
 1.24321;2.15665;0.18043;,
 0.60994;2.41052;-0.22248;,
 0.60994;2.41052;0.22248;,
 -0.13758;2.61016;0.25839;,
 2.05530;1.03014;0.07030;,
 1.72697;1.22214;0.13715;,
 1.72697;1.22214;-0.13715;,
 1.24321;1.40107;0.18043;,
 0.60994;1.47888;0.22248;,
 0.60994;1.47888;-0.22248;,
 -0.13758;1.52817;0.25839;;
 
 106;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 4;2,1,17,18;,
 4;1,4,19,17;,
 4;4,6,20,19;,
 4;6,8,21,20;,
 4;8,10,22,21;,
 4;10,12,23,22;,
 4;12,14,24,23;,
 4;14,16,25,24;,
 4;18,17,26,27;,
 4;17,19,28,26;,
 4;19,20,29,28;,
 4;20,21,30,29;,
 4;21,22,31,30;,
 4;22,23,32,31;,
 4;23,24,33,32;,
 4;24,25,34,33;,
 4;27,26,35,36;,
 4;26,28,37,35;,
 4;28,29,38,37;,
 4;29,30,39,38;,
 4;30,31,40,39;,
 4;31,32,41,40;,
 4;32,33,42,41;,
 4;33,34,43,42;,
 4;36,35,44,45;,
 4;35,37,46,44;,
 4;37,38,47,46;,
 4;38,39,48,47;,
 4;39,40,49,48;,
 4;40,41,50,49;,
 4;41,42,51,50;,
 4;42,43,52,51;,
 4;45,44,53,54;,
 4;44,46,55,53;,
 4;46,47,56,55;,
 4;47,48,57,56;,
 4;48,49,58,57;,
 4;49,50,59,58;,
 4;50,51,60,59;,
 4;51,52,61,60;,
 4;54,53,62,63;,
 4;53,55,64,62;,
 4;55,56,65,64;,
 4;56,57,66,65;,
 4;57,58,67,66;,
 4;58,59,68,67;,
 4;59,60,69,68;,
 4;60,61,70,69;,
 3;63,62,71;,
 3;62,64,72;,
 3;64,65,73;,
 3;65,66,74;,
 3;66,67,75;,
 3;67,68,76;,
 3;68,69,77;,
 3;69,70,78;,
 4;79,80,81,82;,
 4;80,83,84,81;,
 4;83,85,86,84;,
 4;85,87,88,86;,
 4;87,89,90,88;,
 4;89,91,92,90;,
 4;91,93,94,92;,
 4;93,95,96,94;,
 3;97,80,79;,
 3;98,83,80;,
 3;99,85,83;,
 3;100,87,85;,
 3;101,89,87;,
 3;102,91,89;,
 3;103,93,91;,
 3;104,95,93;,
 3;105,82,81;,
 3;106,81,84;,
 3;107,84,86;,
 3;108,86,88;,
 3;109,88,90;,
 3;110,90,92;,
 3;111,92,94;,
 3;112,94,96;,
 4;113,114,115,116;,
 4;114,117,118,115;,
 4;117,119,120,118;,
 4;119,121,122,120;,
 4;123,124,125,126;,
 4;127,128,129,130;,
 4;128,131,132,129;,
 4;131,133,134,132;,
 4;133,135,136,134;,
 4;137,138,139,140;,
 4;137,141,142,143;,
 4;141,131,144,142;,
 4;131,145,146,144;,
 4;145,124,147,146;,
 4;148,149,150,140;,
 4;149,151,132,150;,
 4;151,152,153,132;,
 4;152,154,125,153;;
 
 MeshMaterialList {
  1;
  106;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  88;
  -0.000001;1.000000;0.000000;,
  -0.000001;0.924302;0.381662;,
  -0.195751;0.941524;0.274251;,
  -0.281399;0.959591;0.000000;,
  -0.195751;0.941524;-0.274251;,
  -0.000001;0.924302;-0.381662;,
  0.195750;0.941525;-0.274248;,
  0.281400;0.959591;0.000000;,
  0.195750;0.941525;0.274248;,
  -0.000002;0.708185;0.706027;,
  -0.386560;0.757857;0.525570;,
  -0.578557;0.815642;0.000000;,
  -0.386560;0.757857;-0.525570;,
  -0.000002;0.708185;-0.706027;,
  0.386561;0.757859;-0.525567;,
  0.578559;0.815641;0.000000;,
  0.386561;0.757859;0.525567;,
  -0.000001;0.383657;0.923476;,
  -0.544691;0.434748;0.717151;,
  -0.864098;0.503323;0.000000;,
  -0.544691;0.434748;-0.717151;,
  -0.000001;0.383657;-0.923476;,
  0.544692;0.434749;-0.717150;,
  0.864098;0.503323;0.000000;,
  0.544692;0.434749;0.717150;,
  -0.000001;-0.000003;1.000000;,
  -0.610162;-0.000003;0.792276;,
  -1.000000;-0.000004;0.000000;,
  -0.610162;-0.000003;-0.792276;,
  -0.000001;-0.000003;-1.000000;,
  0.610162;-0.000003;-0.792277;,
  1.000000;-0.000004;0.000000;,
  0.610162;-0.000003;0.792277;,
  -0.000001;-0.383652;0.923478;,
  -0.544693;-0.434744;0.717153;,
  -0.864100;-0.503320;0.000000;,
  -0.544692;-0.434744;-0.717153;,
  -0.000001;-0.383652;-0.923478;,
  0.544694;-0.434744;-0.717152;,
  0.864101;-0.503319;0.000000;,
  0.544694;-0.434744;0.717152;,
  -0.000002;-0.708189;0.706023;,
  -0.386558;-0.757860;0.525568;,
  -0.578553;-0.815645;0.000000;,
  -0.386558;-0.757860;-0.525568;,
  -0.000002;-0.708189;-0.706023;,
  0.386560;-0.757861;-0.525564;,
  0.578557;-0.815642;0.000000;,
  0.386560;-0.757861;0.525564;,
  -0.000001;-0.924307;0.381651;,
  -0.195744;-0.941528;0.274243;,
  -0.281390;-0.959594;0.000000;,
  -0.195744;-0.941528;-0.274243;,
  -0.000001;-0.924307;-0.381651;,
  0.195744;-0.941529;-0.274239;,
  0.281391;-0.959593;0.000000;,
  0.195744;-0.941529;0.274239;,
  -0.000001;-1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.000008;0.000000;1.000000;,
  -0.707113;0.000000;0.707101;,
  -1.000000;0.000000;0.000000;,
  -0.707113;0.000000;-0.707101;,
  -0.000008;0.000000;-1.000000;,
  0.707107;0.000000;-0.707106;,
  1.000000;0.000000;0.000000;,
  0.707107;0.000000;0.707106;,
  0.000000;-1.000000;0.000000;,
  0.199518;0.000000;0.979894;,
  0.144539;0.000000;0.989499;,
  0.077688;0.000000;0.996978;,
  0.057122;0.000000;0.998367;,
  0.047983;0.000000;0.998848;,
  0.057122;0.000000;-0.998367;,
  0.077688;0.000000;-0.996978;,
  0.144539;0.000000;-0.989499;,
  0.199518;0.000000;-0.979894;,
  0.047983;0.000000;-0.998848;,
  0.954169;0.299269;0.000000;,
  0.718394;0.695636;0.000000;,
  0.488748;0.872425;0.000000;,
  0.315634;0.948881;0.000000;,
  0.258026;0.966138;0.000000;,
  -0.504812;-0.863230;0.000000;,
  -0.427492;-0.904019;0.000000;,
  -0.236015;-0.971749;0.000000;,
  -0.093911;-0.995581;0.000000;,
  -0.065796;-0.997833;0.000000;;
  106;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,7,6;,
  3;0,8,7;,
  3;0,1,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,8,16,15;,
  4;8,1,9,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,16,24,23;,
  4;16,9,17,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,24,32,31;,
  4;24,17,25,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,32,40,39;,
  4;32,25,33,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,40,48,47;,
  4;40,33,41,48;,
  4;41,42,50,49;,
  4;42,43,51,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;47,48,56,55;,
  4;48,41,49,56;,
  3;49,50,57;,
  3;50,51,57;,
  3;51,52,57;,
  3;52,53,57;,
  3;53,54,57;,
  3;54,55,57;,
  3;55,56,57;,
  3;56,49,57;,
  4;59,60,60,59;,
  4;60,61,61,60;,
  4;61,62,62,61;,
  4;62,63,63,62;,
  4;63,64,64,63;,
  4;64,65,65,64;,
  4;65,66,66,65;,
  4;66,59,59,66;,
  3;58,58,58;,
  3;58,58,58;,
  3;58,58,58;,
  3;58,58,58;,
  3;58,58,58;,
  3;58,58,58;,
  3;58,58,58;,
  3;58,58,58;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  3;67,67,67;,
  4;68,69,69,68;,
  4;69,70,70,69;,
  4;70,71,71,70;,
  4;71,72,72,71;,
  4;61,61,61,61;,
  4;77,73,73,77;,
  4;73,74,74,73;,
  4;74,75,75,74;,
  4;75,76,76,75;,
  4;78,78,65,65;,
  4;78,79,79,78;,
  4;79,80,80,79;,
  4;80,81,81,80;,
  4;81,82,82,81;,
  4;83,84,84,83;,
  4;84,85,85,84;,
  4;85,86,86,85;,
  4;86,87,87,86;;
 }
 MeshTextureCoords {
  155;
  0.062500;0.000000;,
  0.125000;0.125000;,
  0.000000;0.125000;,
  0.187500;0.000000;,
  0.250000;0.125000;,
  0.312500;0.000000;,
  0.375000;0.125000;,
  0.437500;0.000000;,
  0.500000;0.125000;,
  0.562500;0.000000;,
  0.625000;0.125000;,
  0.687500;0.000000;,
  0.750000;0.125000;,
  0.812500;0.000000;,
  0.875000;0.125000;,
  0.937500;0.000000;,
  1.000000;0.125000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.125000;0.375000;,
  0.000000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.625000;,
  0.000000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.125000;0.875000;,
  0.000000;0.875000;,
  0.250000;0.875000;,
  0.375000;0.875000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.750000;0.875000;,
  0.875000;0.875000;,
  1.000000;0.875000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
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
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.500000;0.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  1.000000;0.000000;;
 }
}
