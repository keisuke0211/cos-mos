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
 183;
 8.31957;29.25567;4.31960;,
 11.91357;25.81358;4.70182;,
 15.01178;31.19665;4.90513;,
 8.57211;34.81396;5.52887;,
 8.57211;34.81396;-5.52887;,
 14.17020;30.98808;-6.10882;,
 11.91357;25.81358;-4.70182;,
 8.31957;29.25567;-4.31959;,
 9.80568;23.72437;0.00000;,
 13.44070;19.45037;0.00000;,
 13.86845;21.45223;4.70182;,
 17.34858;29.01536;5.20767;,
 19.40734;30.70978;3.13917;,
 15.14091;34.94626;3.13917;,
 20.23947;31.83219;0.00000;,
 16.31252;35.93457;0.00000;,
 19.40734;31.73261;-3.13917;,
 15.14091;34.94626;-3.13917;,
 17.34858;29.01536;-5.20767;,
 14.17020;30.98808;-6.10882;,
 17.34858;29.01536;-5.20767;,
 13.86845;21.45223;-4.70182;,
 15.87502;17.25904;0.00000;,
 17.33335;18.79314;4.58602;,
 19.32424;22.49681;6.48561;,
 21.41860;25.91613;4.58602;,
 21.87082;28.15860;0.00000;,
 21.41860;25.91613;-4.58602;,
 19.32424;22.49681;-6.48560;,
 17.33335;18.79314;-4.58602;,
 13.86845;21.45223;-4.70182;,
 17.33335;18.79314;-4.58602;,
 19.04366;11.34259;0.00000;,
 19.97404;12.30376;4.17139;,
 24.48502;14.22448;4.46165;,
 27.32465;16.49229;3.46845;,
 28.55014;17.29148;0.00000;,
 27.32465;16.49229;-3.46845;,
 23.53230;14.62421;-5.89922;,
 19.97404;12.30376;-4.17139;,
 19.97404;12.30376;-4.17139;,
 20.25729;6.45115;0.00000;,
 21.66948;6.06950;4.29810;,
 26.10928;8.67633;4.59718;,
 29.14241;9.59819;3.25070;,
 30.39875;9.98004;0.00000;,
 29.14241;9.59819;-3.25070;,
 26.10928;8.67633;-4.59718;,
 21.79872;6.91963;-3.98828;,
 21.79872;6.91963;-3.98828;,
 19.84576;-0.00001;-0.00000;,
 22.06298;-0.00001;5.35283;,
 27.41581;-0.00001;7.57004;,
 32.76861;-0.00002;5.35283;,
 34.98587;-0.00002;-0.00000;,
 32.76861;-0.00002;-5.35283;,
 27.41581;-0.00001;-7.57004;,
 22.06298;-0.00001;-5.35283;,
 22.06298;-0.00001;-5.35283;,
 19.84576;-0.00001;-0.00000;,
 0.00000;30.03461;0.00000;,
 0.00000;32.18645;4.25544;,
 -4.32804;30.88887;4.31960;,
 -4.01212;28.74656;0.00000;,
 0.00000;37.98423;6.01810;,
 -5.06845;37.39806;5.52887;,
 0.00000;41.87424;4.25545;,
 -5.93531;40.90320;3.90951;,
 0.00000;43.48559;0.00000;,
 -6.20870;42.35506;0.00000;,
 0.00000;41.87424;-4.25544;,
 -5.93531;40.90320;-3.90950;,
 0.00000;37.98423;-6.01810;,
 -5.06845;37.39806;-5.52887;,
 0.00000;32.18645;-4.25544;,
 -4.32804;30.88887;-4.31959;,
 0.00000;32.18645;-4.25544;,
 -4.32804;30.88887;-4.31959;,
 -8.31957;29.25567;4.31960;,
 -7.23491;26.50389;0.00000;,
 -8.57211;34.81396;5.52887;,
 -10.65859;38.11578;3.90951;,
 -11.19484;39.48340;0.00000;,
 -10.65859;38.11578;-3.90950;,
 -8.57211;34.81396;-5.52887;,
 -8.31957;29.25567;-4.31959;,
 -8.31957;29.25567;-4.31959;,
 -11.91357;25.81358;4.70182;,
 -9.80568;23.72437;0.00000;,
 -15.01178;31.19665;4.90513;,
 -15.14091;34.94626;3.13917;,
 -16.31252;35.93457;0.00000;,
 -15.14091;34.94626;-3.13917;,
 -14.17020;30.98808;-6.10882;,
 -8.57211;34.81396;-5.52887;,
 -11.91357;25.81358;-4.70182;,
 -14.17020;30.98808;-6.10882;,
 -13.86845;21.45223;4.70182;,
 -13.44070;19.45037;0.00000;,
 -19.40734;30.70978;3.13917;,
 -17.34858;29.01536;5.20767;,
 -20.23947;31.83219;0.00000;,
 -19.40734;31.73261;-3.13917;,
 -13.86845;21.45223;-4.70182;,
 4.01212;28.74656;0.00000;,
 4.32804;30.88887;4.31960;,
 5.06845;37.39806;5.52887;,
 5.93531;40.90320;3.90951;,
 6.20870;42.35506;0.00000;,
 5.93531;40.90320;-3.90950;,
 5.06845;37.39806;-5.52887;,
 4.32804;30.88887;-4.31959;,
 4.32804;30.88887;-4.31959;,
 7.23491;26.50389;0.00000;,
 8.31957;29.25567;4.31960;,
 8.57211;34.81396;5.52887;,
 10.65859;38.11578;3.90951;,
 11.19484;39.48340;0.00000;,
 10.65859;38.11578;-3.90950;,
 8.57211;34.81396;-5.52887;,
 8.31957;29.25567;-4.31959;,
 8.31957;29.25567;-4.31959;,
 9.80568;23.72437;0.00000;,
 11.91357;25.81358;4.70182;,
 15.01178;31.19665;4.90513;,
 15.14091;34.94626;3.13917;,
 16.31252;35.93457;0.00000;,
 15.14091;34.94626;-3.13917;,
 14.17020;30.98808;-6.10882;,
 11.91357;25.81358;-4.70182;,
 -11.91357;25.81358;4.70182;,
 -15.01178;31.19665;4.90513;,
 -17.34858;29.01536;5.20767;,
 -13.86845;21.45223;4.70182;,
 -15.14091;34.94626;-3.13917;,
 -14.17020;30.98808;-6.10882;,
 -17.34858;29.01536;-5.20767;,
 -19.40734;31.73261;-3.13917;,
 -11.91357;25.81358;-4.70182;,
 -13.86845;21.45223;-4.70182;,
 -13.44070;19.45037;0.00000;,
 -17.33335;18.79314;4.58602;,
 -15.87502;17.25904;0.00000;,
 -19.32424;22.49681;6.48561;,
 -19.40734;30.70978;3.13917;,
 -21.41860;25.91613;4.58602;,
 -20.23947;31.83219;0.00000;,
 -21.87082;28.15860;0.00000;,
 -19.40734;31.73261;-3.13917;,
 -21.41860;25.91613;-4.58602;,
 -17.34858;29.01536;-5.20767;,
 -19.32424;22.49681;-6.48560;,
 -17.33335;18.79314;-4.58602;,
 -19.32424;22.49681;-6.48560;,
 -19.97404;12.30376;4.17139;,
 -19.04366;11.34259;0.00000;,
 -24.48502;14.22448;4.46165;,
 -27.32465;16.49229;3.46845;,
 -28.55014;17.29148;0.00000;,
 -27.32465;16.49229;-3.46845;,
 -23.53230;14.62421;-5.89922;,
 -19.97404;12.30376;-4.17139;,
 -23.53230;14.62421;-5.89922;,
 -21.66948;6.06950;4.29810;,
 -20.25729;6.45115;0.00000;,
 -26.10928;8.67633;4.59718;,
 -29.14241;9.59819;3.25070;,
 -30.39875;9.98004;0.00000;,
 -29.14241;9.59819;-3.25070;,
 -26.10928;8.67633;-4.59718;,
 -21.79872;6.91963;-3.98828;,
 -26.10928;8.67633;-4.59718;,
 -22.06298;-0.00001;5.35283;,
 -19.84576;-0.00001;0.00000;,
 -27.41581;-0.00001;7.57004;,
 -32.76861;-0.00002;5.35283;,
 -34.98587;-0.00002;0.00000;,
 -32.76861;-0.00002;-5.35283;,
 -27.41581;-0.00001;-7.57004;,
 -22.06298;-0.00001;-5.35283;,
 -27.41581;-0.00001;-7.57004;,
 -22.06298;-0.00001;-5.35283;,
 -19.84576;-0.00001;0.00000;;
 
 134;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,1;,
 4;1,10,11,2;,
 4;2,11,12,13;,
 4;13,12,14,15;,
 4;15,14,16,17;,
 4;17,16,18,19;,
 4;5,20,21,6;,
 4;6,21,9,8;,
 4;9,22,23,10;,
 4;10,23,24,11;,
 4;11,24,25,12;,
 4;12,25,26,14;,
 4;14,26,27,16;,
 4;16,27,28,18;,
 4;18,28,29,30;,
 4;21,31,22,9;,
 4;22,32,33,23;,
 4;23,33,34,24;,
 4;24,34,35,25;,
 4;25,35,36,26;,
 4;26,36,37,27;,
 4;27,37,38,28;,
 4;28,38,39,29;,
 4;31,40,32,22;,
 4;32,41,42,33;,
 4;33,42,43,34;,
 4;34,43,44,35;,
 4;35,44,45,36;,
 4;36,45,46,37;,
 4;37,46,47,38;,
 4;38,47,48,39;,
 4;40,49,41,32;,
 4;41,50,51,42;,
 4;42,51,52,43;,
 4;43,52,53,44;,
 4;44,53,54,45;,
 4;45,54,55,46;,
 4;46,55,56,47;,
 4;47,56,57,48;,
 4;49,58,50,41;,
 4;51,54,53,52;,
 4;54,51,50,55;,
 4;55,59,57,56;,
 4;60,61,62,63;,
 4;61,64,65,62;,
 4;64,66,67,65;,
 4;66,68,69,67;,
 4;68,70,71,69;,
 4;70,72,73,71;,
 4;72,74,75,73;,
 4;76,60,63,77;,
 4;63,62,78,79;,
 4;62,65,80,78;,
 4;65,67,81,80;,
 4;67,69,82,81;,
 4;69,71,83,82;,
 4;71,73,84,83;,
 4;73,75,85,84;,
 4;77,63,79,86;,
 4;79,78,87,88;,
 4;78,80,89,87;,
 4;80,81,90,89;,
 4;81,82,91,90;,
 4;82,83,92,91;,
 4;83,84,93,92;,
 4;94,86,95,96;,
 4;86,79,88,95;,
 4;88,87,97,98;,
 4;89,90,99,100;,
 4;90,91,101,99;,
 4;91,92,102,101;,
 4;95,88,98,103;,
 4;60,104,105,61;,
 4;61,105,106,64;,
 4;64,106,107,66;,
 4;66,107,108,68;,
 4;68,108,109,70;,
 4;70,109,110,72;,
 4;72,110,111,74;,
 4;76,112,104,60;,
 4;104,113,114,105;,
 4;105,114,115,106;,
 4;106,115,116,107;,
 4;107,116,117,108;,
 4;108,117,118,109;,
 4;109,118,119,110;,
 4;110,119,120,111;,
 4;112,121,113,104;,
 4;113,122,123,114;,
 4;115,124,125,116;,
 4;116,125,126,117;,
 4;117,126,127,118;,
 4;118,127,128,119;,
 4;121,129,122,113;,
 4;130,131,132,133;,
 4;134,135,136,137;,
 4;135,138,139,136;,
 4;140,133,141,142;,
 4;133,132,143,141;,
 4;132,144,145,143;,
 4;144,146,147,145;,
 4;146,148,149,147;,
 4;148,150,151,149;,
 4;136,139,152,153;,
 4;139,140,142,152;,
 4;142,141,154,155;,
 4;141,143,156,154;,
 4;143,145,157,156;,
 4;145,147,158,157;,
 4;147,149,159,158;,
 4;149,151,160,159;,
 4;153,152,161,162;,
 4;152,142,155,161;,
 4;155,154,163,164;,
 4;154,156,165,163;,
 4;156,157,166,165;,
 4;157,158,167,166;,
 4;158,159,168,167;,
 4;159,160,169,168;,
 4;162,161,170,171;,
 4;161,155,164,170;,
 4;164,163,172,173;,
 4;163,165,174,172;,
 4;165,166,175,174;,
 4;166,167,176,175;,
 4;167,168,177,176;,
 4;168,169,178,177;,
 4;171,170,179,180;,
 4;170,164,173,179;,
 4;172,174,175,176;,
 4;176,177,173,172;,
 4;177,178,181,182;;
 
 MeshMaterialList {
  6;
  134;
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
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.561600;0.561600;0.561600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\rock000.png";
   }
  }
  Material {
   0.561600;0.561600;0.561600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\rock000.png";
   }
  }
  Material {
   0.561600;0.561600;0.561600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\rock000.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.260000;0.260000;0.260000;;
   TextureFilename {
    "data\\TEXTURE\\rock000.png";
   }
  }
  Material {
   0.640000;0.640000;0.640000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\rock000.png";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  172;
  -0.058228;-0.098913;0.993391;,
  -0.240359;-0.174465;-0.954877;,
  -0.842041;-0.539414;0.000000;,
  -0.344383;-0.223836;0.911755;,
  0.080382;0.120477;0.989457;,
  0.529342;0.558937;0.638269;,
  0.688934;0.722886;0.052974;,
  0.545479;0.585272;-0.599925;,
  0.062213;0.023903;-0.997777;,
  -0.369741;-0.307725;-0.876697;,
  -0.746892;-0.664945;0.000000;,
  -0.593867;-0.484000;0.642702;,
  0.108457;0.180042;0.977662;,
  0.598924;0.487075;0.635648;,
  0.814853;0.577826;0.046169;,
  0.620091;0.489075;-0.613427;,
  0.135755;0.115187;-0.984024;,
  -0.321840;-0.375997;-0.868933;,
  -0.786766;-0.617251;0.000000;,
  -0.517912;-0.414652;0.748218;,
  0.108258;0.025240;0.993802;,
  0.662898;0.358534;0.657281;,
  0.886051;0.463190;0.019199;,
  0.681555;0.366977;-0.633096;,
  0.058035;0.040175;-0.997506;,
  -0.567529;-0.407525;-0.715426;,
  -0.935479;-0.353103;-0.014039;,
  -0.585691;-0.257671;0.768487;,
  0.143150;-0.009778;0.989653;,
  0.668510;0.240592;0.703711;,
  0.924502;0.381178;0.000000;,
  0.703563;0.253700;-0.663804;,
  0.108971;-0.047434;-0.992912;,
  -0.634796;-0.306648;-0.709225;,
  -0.993241;-0.114487;-0.019098;,
  -0.814113;0.001781;0.580704;,
  0.076348;0.166495;0.983082;,
  0.657885;0.283825;0.697589;,
  0.942503;0.334198;0.000000;,
  0.684413;0.272140;-0.676401;,
  0.080045;0.114013;-0.990249;,
  -0.668583;-0.054109;-0.741666;,
  -0.998954;0.044673;-0.009776;,
  -0.690512;0.135699;0.710478;,
  0.013525;0.286681;0.957931;,
  0.633049;0.389261;0.669122;,
  0.907078;0.420962;0.000000;,
  0.633049;0.389261;-0.669122;,
  0.010076;0.293941;-0.955770;,
  -0.685030;0.151133;-0.712666;,
  -0.090002;-0.004570;0.995931;,
  -0.762968;-0.488760;-0.423077;,
  -0.067267;0.003342;0.997729;,
  -0.000001;-1.000000;0.000001;,
  -0.000001;-1.000000;0.000001;,
  -0.000001;-1.000000;-0.000001;,
  -0.000001;-1.000000;-0.000001;,
  0.000000;-1.000000;-0.000000;,
  -0.000000;-0.611367;0.791347;,
  0.000000;0.088616;0.996066;,
  0.000000;0.724131;0.689662;,
  0.000000;1.000000;-0.000000;,
  0.000000;0.724132;-0.689662;,
  -0.000000;0.088617;-0.996066;,
  0.000000;-0.611367;-0.791347;,
  0.387039;-0.922063;-0.000000;,
  0.217651;-0.563403;0.796997;,
  -0.063029;0.083450;0.994517;,
  -0.270401;0.674602;0.686874;,
  -0.347738;0.937592;-0.000000;,
  -0.270401;0.674603;-0.686873;,
  -0.063029;0.083450;-0.994517;,
  0.217650;-0.563404;-0.796997;,
  0.519729;-0.677824;0.520035;,
  0.329647;-0.450585;0.829642;,
  -0.098916;0.097107;0.990346;,
  -0.421433;0.596568;0.683009;,
  -0.549542;0.835466;-0.000000;,
  -0.416177;0.612142;-0.672368;,
  -0.049418;0.097898;-0.993969;,
  0.373094;-0.462891;-0.804072;,
  0.793454;-0.576118;0.196264;,
  0.535612;-0.434233;0.724266;,
  -0.222989;0.233513;0.946439;,
  -0.497124;0.563536;0.659768;,
  -0.643332;0.765125;0.026605;,
  -0.523738;0.647438;-0.553645;,
  -0.036665;0.117933;-0.992344;,
  0.580100;-0.444511;-0.682564;,
  0.842041;-0.539414;-0.000000;,
  0.762968;-0.488760;0.423077;,
  -0.375884;0.444695;0.812993;,
  -0.529342;0.558937;0.638269;,
  -0.688934;0.722886;0.052974;,
  -0.631006;0.705610;-0.322408;,
  0.762968;-0.488760;-0.423077;,
  -0.387039;-0.922063;-0.000000;,
  -0.217651;-0.563403;0.796997;,
  0.063029;0.083450;0.994517;,
  0.270401;0.674602;0.686874;,
  0.347738;0.937592;-0.000000;,
  0.270401;0.674603;-0.686873;,
  0.063029;0.083450;-0.994517;,
  -0.217650;-0.563404;-0.796997;,
  -0.519729;-0.677824;0.520035;,
  -0.409813;-0.553176;0.725293;,
  0.152456;0.164283;0.974560;,
  0.421433;0.596568;0.683009;,
  0.549542;0.835466;-0.000000;,
  0.416177;0.612142;-0.672368;,
  0.149475;0.190006;-0.970338;,
  -0.409813;-0.553176;-0.725293;,
  -0.618603;-0.579605;0.530460;,
  0.318190;0.320075;0.892360;,
  0.463086;0.567384;0.680901;,
  0.594581;0.804036;-0.000000;,
  0.452682;0.598919;-0.660587;,
  0.307689;0.391033;-0.867422;,
  -0.618604;-0.579605;-0.530460;,
  0.519729;-0.677823;-0.520036;,
  0.618604;-0.579605;-0.530460;,
  -0.519729;-0.677823;-0.520036;,
  0.090002;-0.004570;0.995931;,
  -0.406053;0.407102;-0.818162;,
  -0.220969;0.128900;-0.966725;,
  -0.006338;-0.166539;-0.986015;,
  0.634971;-0.772536;-0.000000;,
  0.188990;-0.068338;0.979598;,
  -0.008539;0.078217;0.996900;,
  -0.664248;0.408058;0.626309;,
  -0.908340;0.416509;0.037928;,
  -0.596476;0.395993;-0.698145;,
  -0.135755;0.115187;-0.984024;,
  0.321840;-0.375997;-0.868933;,
  0.786766;-0.617251;-0.000000;,
  0.517912;-0.414652;0.748218;,
  -0.108258;0.025240;0.993802;,
  -0.662898;0.358534;0.657281;,
  -0.886051;0.463190;0.019199;,
  -0.681555;0.366977;-0.633096;,
  -0.058035;0.040175;-0.997506;,
  0.567529;-0.407525;-0.715426;,
  0.935479;-0.353103;-0.014039;,
  0.585691;-0.257671;0.768487;,
  -0.143150;-0.009778;0.989653;,
  -0.668510;0.240592;0.703711;,
  -0.924502;0.381178;0.000000;,
  -0.703563;0.253700;-0.663804;,
  -0.108971;-0.047434;-0.992912;,
  0.634796;-0.306648;-0.709225;,
  0.993241;-0.114487;-0.019098;,
  0.814113;0.001781;0.580704;,
  -0.076348;0.166495;0.983082;,
  -0.657885;0.283825;0.697589;,
  -0.942503;0.334198;0.000000;,
  -0.684413;0.272140;-0.676401;,
  -0.080045;0.114013;-0.990249;,
  0.668583;-0.054109;-0.741666;,
  0.998954;0.044673;-0.009776;,
  0.690512;0.135699;0.710478;,
  -0.013525;0.286681;0.957931;,
  -0.633049;0.389261;0.669122;,
  -0.907078;0.420962;0.000000;,
  -0.633049;0.389261;-0.669122;,
  -0.010076;0.293941;-0.955770;,
  0.685030;0.151133;-0.712666;,
  0.583697;-0.710154;0.393674;,
  0.067267;0.003342;0.997729;,
  0.000001;-1.000000;0.000001;,
  0.000001;-1.000000;0.000001;,
  0.000001;-1.000000;-0.000001;,
  0.000001;-1.000000;-0.000001;;
  134;
  4;0,3,4,0;,
  4;1,8,9,1;,
  4;2,10,11,3;,
  4;3,50,12,4;,
  4;4,12,13,5;,
  4;5,13,14,6;,
  4;6,14,15,7;,
  4;7,15,16,8;,
  4;8,16,17,9;,
  4;9,51,10,2;,
  4;10,18,19,11;,
  4;11,19,20,12;,
  4;12,20,21,13;,
  4;13,21,22,14;,
  4;14,22,23,15;,
  4;15,23,24,16;,
  4;16,24,25,17;,
  4;17,25,18,10;,
  4;18,26,27,19;,
  4;19,27,28,20;,
  4;20,28,29,21;,
  4;21,29,30,22;,
  4;22,30,31,23;,
  4;23,31,32,24;,
  4;24,32,33,25;,
  4;25,33,26,18;,
  4;26,34,35,27;,
  4;27,52,36,28;,
  4;28,36,37,29;,
  4;29,37,38,30;,
  4;30,38,39,31;,
  4;31,39,40,32;,
  4;32,40,41,33;,
  4;33,41,34,26;,
  4;34,42,43,35;,
  4;35,43,44,36;,
  4;36,44,45,37;,
  4;37,45,46,38;,
  4;38,46,47,39;,
  4;39,47,48,40;,
  4;40,48,49,41;,
  4;41,49,42,34;,
  4;53,53,54,54;,
  4;53,53,55,55;,
  4;55,55,56,56;,
  4;57,58,66,65;,
  4;58,59,67,66;,
  4;59,60,68,67;,
  4;60,61,69,68;,
  4;61,62,70,69;,
  4;62,63,71,70;,
  4;63,64,72,71;,
  4;64,57,65,72;,
  4;65,66,74,73;,
  4;66,67,75,74;,
  4;67,68,76,75;,
  4;68,69,77,76;,
  4;69,70,78,77;,
  4;70,71,79,78;,
  4;71,72,80,79;,
  4;72,65,119,80;,
  4;73,74,82,81;,
  4;74,75,83,82;,
  4;75,76,84,83;,
  4;76,77,85,84;,
  4;77,78,86,85;,
  4;78,79,87,86;,
  4;79,80,88,87;,
  4;80,119,120,88;,
  4;81,82,90,89;,
  4;83,84,92,91;,
  4;84,85,93,92;,
  4;85,86,94,93;,
  4;88,81,89,95;,
  4;57,96,97,58;,
  4;58,97,98,59;,
  4;59,98,99,60;,
  4;60,99,100,61;,
  4;61,100,101,62;,
  4;62,101,102,63;,
  4;63,102,103,64;,
  4;64,103,96,57;,
  4;96,104,105,97;,
  4;97,105,106,98;,
  4;98,106,107,99;,
  4;99,107,108,100;,
  4;100,108,109,101;,
  4;101,109,110,102;,
  4;102,110,111,103;,
  4;103,111,121,96;,
  4;104,112,112,105;,
  4;106,113,114,107;,
  4;107,114,115,108;,
  4;108,115,116,109;,
  4;109,116,117,110;,
  4;111,118,118,121;,
  4;122,122,128,127;,
  4;123,124,132,131;,
  4;124,125,133,132;,
  4;126,166,135,134;,
  4;127,128,136,135;,
  4;128,129,137,136;,
  4;129,130,138,137;,
  4;130,131,139,138;,
  4;131,132,140,139;,
  4;132,133,141,140;,
  4;133,126,134,141;,
  4;134,135,143,142;,
  4;135,136,144,143;,
  4;136,137,145,144;,
  4;137,138,146,145;,
  4;138,139,147,146;,
  4;139,140,148,147;,
  4;140,141,149,148;,
  4;141,134,142,149;,
  4;142,143,151,150;,
  4;143,144,152,167;,
  4;144,145,153,152;,
  4;145,146,154,153;,
  4;146,147,155,154;,
  4;147,148,156,155;,
  4;148,149,157,156;,
  4;149,142,150,157;,
  4;150,151,159,158;,
  4;151,152,160,159;,
  4;152,153,161,160;,
  4;153,154,162,161;,
  4;154,155,163,162;,
  4;155,156,164,163;,
  4;156,157,165,164;,
  4;157,150,158,165;,
  4;168,169,169,168;,
  4;168,170,170,168;,
  4;170,171,171,170;;
 }
 MeshTextureCoords {
  183;
  0.659655;-0.282691;,
  0.630724;-0.049229;,
  0.497026;-0.223234;,
  0.601749;-0.526943;,
  0.898251;-0.526943;,
  0.980399;-0.232307;,
  0.869276;-0.049229;,
  0.840344;-0.282691;,
  0.750000;-0.001542;,
  0.750000;0.270205;,
  0.633117;0.189617;,
  0.462919;-0.073988;,
  0.359297;-0.105010;,
  0.415190;-0.388910;,
  0.250000;-0.137212;,
  0.250000;-0.407644;,
  0.149235;-0.150976;,
  0.084810;-0.388910;,
  0.037081;-0.073988;,
  -0.019601;-0.232307;,
  1.037081;-0.073988;,
  0.866883;0.189617;,
  0.750000;0.422039;,
  0.598332;0.385061;,
  0.496224;0.262257;,
  0.395479;0.154499;,
  0.250000;0.063635;,
  0.104521;0.154499;,
  0.003776;0.262257;,
  -0.098332;0.385061;,
  -0.133117;0.189617;,
  0.901668;0.385061;,
  0.750000;0.757374;,
  0.616127;0.734572;,
  0.459315;0.747130;,
  0.350983;0.707456;,
  0.250000;0.698402;,
  0.149017;0.707456;,
  0.013055;0.708284;,
  -0.116127;0.734572;,
  0.883873;0.734572;,
  0.750000;1.003795;,
  0.636032;1.051900;,
  0.493296;1.032064;,
  0.373225;1.057117;,
  0.250000;1.067494;,
  0.126775;1.057117;,
  0.006704;1.032064;,
  -0.133711;1.016528;,
  0.866289;1.016528;,
  0.750000;1.284688;,
  0.648743;1.333287;,
  0.549538;1.450613;,
  0.433521;1.567939;,
  0.250000;1.616538;,
  0.066479;1.567939;,
  -0.049538;1.450613;,
  -0.148743;1.333287;,
  0.851257;1.333287;,
  -0.250000;1.284688;,
  0.250000;0.511255;,
  0.393406;0.510316;,
  0.369492;0.727276;,
  0.250000;0.712415;,
  0.561029;0.507786;,
  0.552320;0.761455;,
  0.655658;0.506089;,
  0.650246;0.803267;,
  0.750000;0.505386;,
  0.750000;0.816302;,
  0.844342;0.506089;,
  0.849753;0.803267;,
  0.938971;0.507786;,
  0.947680;0.761455;,
  1.106594;0.510316;,
  1.130508;0.727276;,
  0.106594;0.510316;,
  0.130508;0.727276;,
  0.346515;0.927557;,
  0.250000;0.874527;,
  0.480879;0.937758;,
  0.594876;1.040638;,
  0.750000;1.066853;,
  0.905124;1.040638;,
  1.019121;0.937758;,
  1.153485;0.927557;,
  0.153485;0.927557;,
  0.322234;1.108752;,
  0.250000;1.004273;,
  0.386770;1.261308;,
  0.475958;1.266128;,
  0.750000;1.324275;,
  1.024042;1.266128;,
  1.100103;1.219321;,
  0.019121;0.937758;,
  0.177766;1.108752;,
  0.100103;1.219321;,
  0.301628;1.208395;,
  0.250000;1.187882;,
  0.344082;1.481290;,
  0.358840;1.379095;,
  0.250000;1.522405;,
  1.137042;1.480844;,
  0.198372;1.208395;,
  0.250000;0.311218;,
  0.368208;0.294488;,
  0.550028;0.254629;,
  0.648777;0.209758;,
  0.750000;0.195456;,
  0.851223;0.209758;,
  0.949972;0.254629;,
  1.131792;0.294488;,
  0.131792;0.294488;,
  0.250000;0.151063;,
  0.344804;0.095632;,
  0.476649;0.080580;,
  0.589575;-0.025181;,
  0.750000;-0.052589;,
  0.910425;-0.025181;,
  1.023351;0.080580;,
  1.155196;0.095632;,
  0.155196;0.095632;,
  0.250000;0.023743;,
  0.320904;-0.082559;,
  0.382020;-0.239813;,
  0.463054;-0.247905;,
  0.750000;-0.306915;,
  1.036946;-0.247905;,
  1.104235;-0.197644;,
  0.179096;-0.082559;,
  0.346623;-0.075020;,
  0.446400;-0.255699;,
  0.484901;-0.108002;,
  0.346297;0.163947;,
  0.002278;-0.423622;,
  0.063373;-0.263659;,
  0.015099;-0.108002;,
  -0.107182;-0.188944;,
  0.153377;-0.075020;,
  0.153703;0.163947;,
  0.250000;0.246158;,
  0.372230;0.356774;,
  0.250000;0.396334;,
  0.466141;0.229544;,
  0.593152;-0.142407;,
  0.563186;0.117398;,
  0.750000;-0.176221;,
  0.750000;0.024746;,
  0.892818;-0.188944;,
  0.936814;0.117398;,
  1.015099;-0.108002;,
  1.033859;0.229544;,
  0.127770;0.356774;,
  0.033859;0.229544;,
  0.360088;0.706782;,
  0.250000;0.731222;,
  0.495173;0.712926;,
  0.623157;0.668624;,
  0.750000;0.657673;,
  0.876843;0.668624;,
  1.022419;0.674985;,
  0.139912;0.706782;,
  0.022419;0.674985;,
  0.347947;1.025583;,
  0.250000;0.978937;,
  0.467298;0.999035;,
  0.594161;1.019983;,
  0.750000;1.028659;,
  0.905839;1.019983;,
  1.032702;0.999035;,
  0.151626;0.989583;,
  0.032702;0.999035;,
  0.341707;1.309893;,
  0.250000;1.263919;,
  0.433389;1.420882;,
  0.543583;1.531871;,
  0.750000;1.577845;,
  0.956417;1.531871;,
  1.066611;1.420882;,
  0.158293;1.309893;,
  0.066611;1.420882;,
  1.158293;1.309893;,
  1.250000;1.263919;;
 }
}