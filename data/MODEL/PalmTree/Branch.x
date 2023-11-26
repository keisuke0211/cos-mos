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
 100;
 23.98362;-0.92185;-0.23386;,
 23.99083;-1.08706;-0.16536;,
 19.65658;-0.35290;-0.25905;,
 19.64533;-0.09407;-0.36635;,
 23.99380;-1.15550;-0.00000;,
 19.66130;-0.46009;-0.00000;,
 23.99083;-1.08706;0.16536;,
 19.65658;-0.35290;0.25905;,
 23.98362;-0.92185;0.23386;,
 19.64533;-0.09407;0.36635;,
 23.97639;-0.75665;0.16536;,
 19.63401;0.16471;0.25905;,
 23.97338;-0.68821;-0.00000;,
 19.62933;0.27193;0.00000;,
 23.97639;-0.75665;-0.16536;,
 19.63401;0.16471;-0.25905;,
 23.98362;-0.92185;-0.23386;,
 19.64533;-0.09407;-0.36635;,
 13.46963;-0.00448;-0.33408;,
 13.45504;0.32925;-0.47246;,
 13.47564;-0.14275;-0.00000;,
 13.46963;-0.00448;0.33408;,
 13.45504;0.32925;0.47246;,
 13.44048;0.66303;0.33408;,
 13.43443;0.80127;0.00000;,
 13.44048;0.66303;-0.33408;,
 13.45504;0.32925;-0.47246;,
 -0.29442;-0.42588;-0.41400;,
 -0.31249;-0.01230;-0.58548;,
 -0.28697;-0.59720;0.00000;,
 -0.29442;-0.42588;0.41400;,
 -0.31249;-0.01230;0.58548;,
 -0.33055;0.40132;0.41399;,
 -0.33804;0.57263;0.00000;,
 -0.33055;0.40132;-0.41400;,
 -0.31249;-0.01230;-0.58548;,
 28.43011;-2.06393;0.01959;,
 28.43011;-2.06393;0.01959;,
 28.43011;-2.06393;0.01959;,
 28.43011;-2.06393;0.01959;,
 28.43011;-2.06393;0.01959;,
 28.43011;-2.06393;0.01959;,
 28.43011;-2.06393;0.01959;,
 28.43011;-2.06393;0.01959;,
 -0.31249;-0.01230;0.00000;,
 -0.31249;-0.01230;0.00000;,
 -0.31249;-0.01230;0.00000;,
 -0.31249;-0.01230;0.00000;,
 -0.31249;-0.01230;0.00000;,
 -0.31249;-0.01230;0.00000;,
 -0.31249;-0.01230;0.00000;,
 -0.31249;-0.01230;0.00000;,
 22.06312;-0.58383;0.01959;,
 20.10321;-0.08588;0.01959;,
 20.16901;-1.59152;-1.76771;,
 22.36529;-7.50437;-4.76543;,
 22.36529;-7.50437;3.84382;,
 20.16901;-1.59152;0.84611;,
 18.16877;-0.17034;0.01959;,
 18.47087;-7.09088;-4.76543;,
 18.47087;-7.09088;3.84382;,
 13.18373;0.30564;0.01959;,
 11.50974;0.23252;0.01959;,
 11.59413;-1.70032;-1.86224;,
 13.50373;-7.02392;-4.76543;,
 13.50373;-7.02392;3.84382;,
 11.59413;-1.70032;0.94063;,
 9.83577;0.15944;0.01959;,
 10.17368;-7.57947;-4.76543;,
 10.17368;-7.57947;3.84382;,
 26.07502;-5.85947;-4.76543;,
 28.43011;-2.06393;0.01959;,
 3.43120;0.13936;0.01959;,
 6.10713;-7.03026;-4.76543;,
 6.10713;-7.03026;3.84382;,
 26.07502;-5.85947;3.84382;,
 22.06312;-0.58383;0.01959;,
 22.36529;-7.50437;-4.76543;,
 20.16901;-1.59152;-1.76771;,
 20.10321;-0.08588;0.01959;,
 20.16901;-1.59152;0.84611;,
 22.36529;-7.50437;3.84382;,
 18.47087;-7.09088;-4.76543;,
 18.16877;-0.17034;0.01959;,
 18.47087;-7.09088;3.84382;,
 13.18373;0.30564;0.01959;,
 13.50373;-7.02392;-4.76543;,
 11.59413;-1.70032;-1.86224;,
 11.50974;0.23252;0.01959;,
 11.59413;-1.70032;0.94063;,
 13.50373;-7.02392;3.84382;,
 10.17368;-7.57947;-4.76543;,
 9.83577;0.15944;0.01959;,
 10.17368;-7.57947;3.84382;,
 28.43011;-2.06393;0.01959;,
 26.07502;-5.85947;-4.76543;,
 3.43120;0.13936;0.01959;,
 6.10713;-7.03026;-4.76543;,
 6.10713;-7.03026;3.84382;,
 26.07502;-5.85947;3.84382;;
 
 80;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;3,2,18,19;,
 4;2,5,20,18;,
 4;5,7,21,20;,
 4;7,9,22,21;,
 4;9,11,23,22;,
 4;11,13,24,23;,
 4;13,15,25,24;,
 4;15,17,26,25;,
 4;19,18,27,28;,
 4;18,20,29,27;,
 4;20,21,30,29;,
 4;21,22,31,30;,
 4;22,23,32,31;,
 4;23,24,33,32;,
 4;24,25,34,33;,
 4;25,26,35,34;,
 3;36,1,0;,
 3;37,4,1;,
 3;38,6,4;,
 3;39,8,6;,
 3;40,10,8;,
 3;41,12,10;,
 3;42,14,12;,
 3;43,16,14;,
 3;44,28,27;,
 3;45,27,29;,
 3;46,29,30;,
 3;47,30,31;,
 3;48,31,32;,
 3;49,32,33;,
 3;50,33,34;,
 3;51,34,35;,
 4;52,53,54,55;,
 4;52,56,57,53;,
 4;53,58,59,54;,
 4;53,57,60,58;,
 4;61,62,63,64;,
 4;61,65,66,62;,
 4;62,67,68,63;,
 4;62,66,69,67;,
 3;52,70,71;,
 3;52,55,70;,
 3;61,59,58;,
 3;61,64,59;,
 3;72,68,67;,
 3;72,73,68;,
 3;72,69,74;,
 3;72,67,69;,
 3;61,60,65;,
 3;61,58,60;,
 3;52,75,56;,
 3;52,71,75;,
 4;76,77,78,79;,
 4;76,79,80,81;,
 4;79,78,82,83;,
 4;79,83,84,80;,
 4;85,86,87,88;,
 4;85,88,89,90;,
 4;88,87,91,92;,
 4;88,92,93,89;,
 3;76,94,95;,
 3;76,95,77;,
 3;85,83,82;,
 3;85,82,86;,
 3;96,92,91;,
 3;96,91,97;,
 3;96,98,93;,
 3;96,93,92;,
 3;85,90,84;,
 3;85,84,83;,
 3;76,81,99;,
 3;76,99,94;;
 
 MeshMaterialList {
  2;
  80;
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
  1;;
  Material {
   0.511373;0.323137;0.109804;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Wood.png";
   }
  }
  Material {
   0.000000;0.614902;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  100;
  -0.125042;-0.701149;-0.701965;,
  0.043732;0.000145;-0.999043;,
  -0.114514;-0.702400;-0.702510;,
  -0.179598;-0.983740;-0.000075;,
  -0.117579;-0.702201;0.702203;,
  0.039348;0.000148;0.999226;,
  0.194081;0.693891;0.693432;,
  0.255665;0.966765;0.000100;,
  0.197004;0.693569;-0.692930;,
  0.023868;0.000388;-0.999715;,
  -0.067658;-0.705889;-0.705083;,
  -0.105073;-0.994465;0.000000;,
  -0.067658;-0.705889;0.705084;,
  0.023868;0.000388;0.999715;,
  0.114444;0.703271;0.701649;,
  0.150928;0.988545;0.000000;,
  0.114445;0.703271;-0.701650;,
  0.012684;0.000393;-0.999919;,
  -0.002760;-0.707241;-0.706967;,
  -0.009138;-0.999958;0.000000;,
  -0.002760;-0.707241;0.706967;,
  0.012684;0.000393;0.999920;,
  0.028047;0.707369;0.706288;,
  0.034344;0.999410;0.000000;,
  0.028047;0.707369;-0.706288;,
  0.008201;0.000316;-0.999966;,
  0.025739;-0.706640;-0.707105;,
  0.033003;-0.999455;0.000000;,
  0.025739;-0.706640;0.707105;,
  0.008201;0.000316;0.999966;,
  -0.009337;0.707182;0.706969;,
  -0.016599;0.999862;0.000000;,
  -0.009337;0.707182;-0.706969;,
  -0.999048;-0.043625;0.000000;,
  -0.131160;-0.700697;0.701299;,
  0.224747;0.688830;0.689204;,
  0.230535;0.688052;-0.688069;,
  -0.999048;-0.043632;0.000000;,
  -0.999048;-0.043630;-0.000029;,
  -0.999049;-0.043600;0.000000;,
  -0.999048;-0.043630;0.000029;,
  -0.999048;-0.043621;0.000017;,
  -0.999047;-0.043638;0.000000;,
  -0.999048;-0.043621;-0.000017;,
  -0.169640;-0.729747;0.662338;,
  -0.065998;-0.488669;-0.869969;,
  -0.018248;-0.456386;-0.889595;,
  0.026242;-0.440456;-0.897391;,
  0.018167;-0.453823;-0.890907;,
  -0.010915;-0.476588;-0.879059;,
  -0.050976;-0.483516;-0.873850;,
  0.043220;-0.481148;-0.875574;,
  0.028082;-0.572589;-0.819362;,
  -0.077426;-0.573289;0.815687;,
  0.039816;-0.570938;0.820027;,
  0.036013;-0.620989;0.782992;,
  -0.065494;-0.587840;0.806322;,
  0.040791;-0.596412;0.801641;,
  -0.064497;-0.675985;0.734087;,
  -0.034783;-0.623788;0.780819;,
  0.036992;-0.655786;0.754040;,
  -0.081416;-0.663369;0.743850;,
  0.065001;-0.620045;0.781869;,
  -0.061785;-0.581750;0.811018;,
  0.098588;-0.566786;0.817945;,
  -0.037935;-0.550236;0.834147;,
  0.169640;0.729747;-0.662338;,
  0.065998;0.488669;0.869969;,
  0.018248;0.456386;0.889595;,
  -0.026242;0.440456;0.897391;,
  -0.018167;0.453823;0.890907;,
  0.010915;0.476588;0.879059;,
  0.050976;0.483516;0.873850;,
  -0.043220;0.481148;0.875574;,
  -0.028082;0.572589;0.819362;,
  0.077426;0.573289;-0.815687;,
  -0.039816;0.570938;-0.820027;,
  -0.036013;0.620989;-0.782992;,
  0.065494;0.587840;-0.806322;,
  -0.040791;0.596412;-0.801641;,
  0.064497;0.675985;-0.734087;,
  0.034783;0.623788;-0.780819;,
  -0.036992;0.655786;-0.754040;,
  0.081416;0.663369;-0.743850;,
  -0.065001;0.620045;-0.781869;,
  0.061785;0.581750;-0.811018;,
  -0.098588;0.566786;-0.817945;,
  0.037935;0.550236;-0.834147;,
  -0.022614;-0.544244;-0.838622;,
  -0.013253;-0.483447;-0.875273;,
  -0.003322;-0.464393;-0.885623;,
  0.005652;-0.439609;-0.898171;,
  -0.032334;-0.466247;-0.884064;,
  -0.151845;-0.653197;-0.741806;,
  0.022614;0.544244;0.838622;,
  0.013253;0.483447;0.875273;,
  0.003322;0.464393;0.885623;,
  -0.005652;0.439609;0.898171;,
  0.032334;0.466247;0.884064;,
  0.151845;0.653197;0.741806;;
  80;
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
  3;0,2,1;,
  3;0,3,2;,
  3;34,4,3;,
  3;34,5,4;,
  3;35,6,5;,
  3;35,7,6;,
  3;36,8,7;,
  3;36,1,8;,
  3;33,37,38;,
  3;33,38,39;,
  3;33,39,40;,
  3;33,40,37;,
  3;33,37,41;,
  3;33,41,42;,
  3;33,42,43;,
  3;33,43,37;,
  4;61,58,58,59;,
  4;88,51,50,50;,
  4;58,62,57,58;,
  4;50,50,49,89;,
  4;63,55,55,56;,
  4;90,48,47,47;,
  4;55,64,54,55;,
  4;47,47,46,91;,
  3;61,60,44;,
  3;61,59,60;,
  3;63,57,62;,
  3;63,56,57;,
  3;65,54,64;,
  3;65,53,54;,
  3;92,46,45;,
  3;92,91,46;,
  3;90,49,48;,
  3;90,89,49;,
  3;88,52,51;,
  3;88,93,52;,
  4;83,81,80,80;,
  4;94,72,72,73;,
  4;80,80,79,84;,
  4;72,95,71,72;,
  4;85,78,77,77;,
  4;96,69,69,70;,
  4;77,77,76,86;,
  4;69,97,68,69;,
  3;83,66,82;,
  3;83,82,81;,
  3;85,84,79;,
  3;85,79,78;,
  3;87,86,76;,
  3;87,76,75;,
  3;98,67,68;,
  3;98,68,97;,
  3;96,70,71;,
  3;96,71,95;,
  3;94,73,74;,
  3;94,74,99;;
 }
 MeshTextureCoords {
  100;
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;0.333333;,
  0.000000;0.333333;,
  0.250000;0.000000;,
  0.250000;0.333333;,
  0.375000;0.000000;,
  0.375000;0.333333;,
  0.500000;0.000000;,
  0.500000;0.333333;,
  0.625000;0.000000;,
  0.625000;0.333333;,
  0.750000;0.000000;,
  0.750000;0.333333;,
  0.875000;0.000000;,
  0.875000;0.333333;,
  1.000000;0.000000;,
  1.000000;0.333333;,
  0.125000;0.666667;,
  0.000000;0.666667;,
  0.250000;0.666667;,
  0.375000;0.666667;,
  0.500000;0.666667;,
  0.625000;0.666667;,
  0.750000;0.666667;,
  0.875000;0.666667;,
  1.000000;0.666667;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;1.000000;,
  0.375000;1.000000;,
  0.500000;1.000000;,
  0.625000;1.000000;,
  0.750000;1.000000;,
  0.875000;1.000000;,
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
  0.857143;0.475980;,
  0.714286;0.475980;,
  0.714286;1.000000;,
  0.857143;1.000000;,
  0.857143;0.000000;,
  0.714286;0.000000;,
  0.571429;0.475980;,
  0.571429;1.000000;,
  0.571429;0.000000;,
  0.428571;0.475980;,
  0.285714;0.475980;,
  0.285714;1.000000;,
  0.428571;1.000000;,
  0.428571;0.000000;,
  0.285714;0.000000;,
  0.142857;0.475980;,
  0.142857;1.000000;,
  0.142857;0.000000;,
  1.000000;1.000000;,
  1.000000;0.475980;,
  0.000000;0.475980;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.857143;0.475980;,
  0.857143;1.000000;,
  0.714286;1.000000;,
  0.714286;0.475980;,
  0.714286;0.000000;,
  0.857143;0.000000;,
  0.571429;1.000000;,
  0.571429;0.475980;,
  0.571429;0.000000;,
  0.428571;0.475980;,
  0.428571;1.000000;,
  0.285714;1.000000;,
  0.285714;0.475980;,
  0.285714;0.000000;,
  0.428571;0.000000;,
  0.142857;1.000000;,
  0.142857;0.475980;,
  0.142857;0.000000;,
  1.000000;0.475980;,
  1.000000;1.000000;,
  0.000000;0.475980;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}