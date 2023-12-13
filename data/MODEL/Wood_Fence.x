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
 204;
 -4.98407;11.13230;-1.70234;,
 -4.37047;11.13230;-1.57276;,
 -4.37047;-0.19425;-1.57276;,
 -4.98407;-0.19425;-1.70234;,
 -3.85027;11.13230;-1.20374;,
 -3.85027;-0.19425;-1.20374;,
 -3.50269;11.13230;-0.65145;,
 -3.50269;-0.19425;-0.65145;,
 -3.38064;11.13230;0.00000;,
 -3.38064;-0.19425;0.00000;,
 -3.50269;11.13230;0.65145;,
 -3.50269;-0.19425;0.65145;,
 -3.85027;11.13230;1.20374;,
 -3.85027;-0.19425;1.20374;,
 -4.37047;11.13230;1.57276;,
 -4.37047;-0.19425;1.57276;,
 -4.98407;11.13230;1.70234;,
 -4.98407;-0.19425;1.70234;,
 -5.59769;11.13230;1.57277;,
 -5.59769;-0.19425;1.57277;,
 -6.11789;11.13230;1.20374;,
 -6.11789;-0.19425;1.20374;,
 -6.46547;11.13230;0.65145;,
 -6.46547;-0.19425;0.65145;,
 -6.58752;11.13230;0.00000;,
 -6.58752;-0.19425;0.00000;,
 -6.46547;11.13230;-0.65145;,
 -6.46547;-0.19425;-0.65145;,
 -6.11789;11.13230;-1.20374;,
 -6.11789;-0.19425;-1.20374;,
 -5.59769;11.13230;-1.57277;,
 -5.59769;-0.19425;-1.57277;,
 -4.98407;11.13230;-1.70234;,
 -4.98407;-0.19425;-1.70234;,
 -4.98407;11.13230;0.00000;,
 -4.37047;11.13230;-1.57276;,
 -4.98407;11.13230;-1.70234;,
 -3.85027;11.13230;-1.20374;,
 -3.50269;11.13230;-0.65145;,
 -3.38064;11.13230;0.00000;,
 -3.50269;11.13230;0.65145;,
 -3.85027;11.13230;1.20374;,
 -4.37047;11.13230;1.57276;,
 -4.98407;11.13230;1.70234;,
 -5.59769;11.13230;1.57277;,
 -6.11789;11.13230;1.20374;,
 -6.46547;11.13230;0.65145;,
 -6.58752;11.13230;0.00000;,
 -6.46547;11.13230;-0.65145;,
 -6.11789;11.13230;-1.20374;,
 -5.59769;11.13230;-1.57277;,
 -4.98407;-0.19425;0.00000;,
 -4.98407;-0.19425;-1.70234;,
 -4.37047;-0.19425;-1.57276;,
 -3.85027;-0.19425;-1.20374;,
 -3.50269;-0.19425;-0.65145;,
 -3.38064;-0.19425;0.00000;,
 -3.50269;-0.19425;0.65145;,
 -3.85027;-0.19425;1.20374;,
 -4.37047;-0.19425;1.57276;,
 -4.98407;-0.19425;1.70234;,
 -5.59769;-0.19425;1.57277;,
 -6.11789;-0.19425;1.20374;,
 -6.46547;-0.19425;0.65145;,
 -6.58752;-0.19425;0.00000;,
 -6.46547;-0.19425;-0.65145;,
 -6.11789;-0.19425;-1.20374;,
 -5.59769;-0.19425;-1.57277;,
 7.97459;5.71578;-1.22486;,
 8.00143;5.40137;-1.13163;,
 -7.81376;5.27438;-1.13163;,
 -7.84062;5.58878;-1.22486;,
 8.02418;5.13482;-0.86611;,
 -7.79101;5.00783;-0.86611;,
 8.03942;4.95672;-0.46873;,
 -7.77580;4.82973;-0.46873;,
 8.04475;4.89419;0.00000;,
 -7.77044;4.76719;0.00000;,
 8.03942;4.95672;0.46873;,
 -7.77580;4.82973;0.46873;,
 8.02418;5.13482;0.86611;,
 -7.79101;5.00783;0.86611;,
 8.00143;5.40137;1.13163;,
 -7.81376;5.27438;1.13163;,
 7.97459;5.71578;1.22486;,
 -7.84062;5.58878;1.22486;,
 7.94774;6.03018;1.13163;,
 -7.86748;5.91081;1.13163;,
 7.92499;6.29673;0.86611;,
 -7.89023;6.18545;0.86611;,
 7.90979;6.47482;0.46873;,
 -7.90543;6.36896;0.46873;,
 7.90445;6.53736;0.00000;,
 -7.91077;6.43340;0.00000;,
 7.90979;6.47482;-0.46873;,
 -7.90543;6.36896;-0.46873;,
 7.92499;6.29673;-0.86611;,
 -7.89023;6.18545;-0.86611;,
 7.94774;6.03018;-1.13163;,
 -7.86748;5.91081;-1.13163;,
 7.97459;5.71578;-1.22486;,
 -7.84062;5.58878;-1.22486;,
 7.97459;5.71578;0.00000;,
 8.00143;5.40137;-1.13163;,
 7.97459;5.71578;-1.22486;,
 8.02418;5.13482;-0.86611;,
 8.03942;4.95672;-0.46873;,
 8.04475;4.89419;0.00000;,
 8.03942;4.95672;0.46873;,
 8.02418;5.13482;0.86611;,
 8.00143;5.40137;1.13163;,
 7.97459;5.71578;1.22486;,
 7.94774;6.03018;1.13163;,
 7.92499;6.29673;0.86611;,
 7.90979;6.47482;0.46873;,
 7.90445;6.53736;0.00000;,
 7.90979;6.47482;-0.46873;,
 7.92499;6.29673;-0.86611;,
 7.94774;6.03018;-1.13163;,
 -7.84062;5.58878;0.00000;,
 -7.84062;5.58878;-1.22486;,
 -7.81376;5.27438;-1.13163;,
 -7.79101;5.00783;-0.86611;,
 -7.77580;4.82973;-0.46873;,
 -7.77044;4.76719;0.00000;,
 -7.77580;4.82973;0.46873;,
 -7.79101;5.00783;0.86611;,
 -7.81376;5.27438;1.13163;,
 -7.84062;5.58878;1.22486;,
 -7.86748;5.91081;1.13163;,
 -7.89023;6.18545;0.86611;,
 -7.90543;6.36896;0.46873;,
 -7.91077;6.43340;0.00000;,
 -7.90543;6.36896;-0.46873;,
 -7.89023;6.18545;-0.86611;,
 -7.86748;5.91081;-1.13163;,
 5.03985;11.13230;-1.70234;,
 5.65347;11.13230;-1.57276;,
 5.65347;-0.19425;-1.57276;,
 5.03985;-0.19425;-1.70234;,
 6.17366;11.13230;-1.20374;,
 6.17366;-0.19425;-1.20374;,
 6.52124;11.13230;-0.65145;,
 6.52124;-0.19425;-0.65145;,
 6.64330;11.13230;0.00000;,
 6.64330;-0.19425;-0.00000;,
 6.52124;11.13230;0.65145;,
 6.52124;-0.19425;0.65145;,
 6.17366;11.13230;1.20374;,
 6.17366;-0.19425;1.20374;,
 5.65347;11.13230;1.57276;,
 5.65347;-0.19425;1.57276;,
 5.03985;11.13230;1.70234;,
 5.03985;-0.19425;1.70234;,
 4.42624;11.13230;1.57277;,
 4.42624;-0.19425;1.57277;,
 3.90604;11.13230;1.20374;,
 3.90604;-0.19425;1.20374;,
 3.55846;11.13230;0.65145;,
 3.55846;-0.19425;0.65145;,
 3.43640;11.13230;0.00000;,
 3.43640;-0.19425;-0.00000;,
 3.55846;11.13230;-0.65145;,
 3.55846;-0.19425;-0.65145;,
 3.90604;11.13230;-1.20374;,
 3.90604;-0.19425;-1.20374;,
 4.42624;11.13230;-1.57277;,
 4.42624;-0.19425;-1.57277;,
 5.03985;11.13230;-1.70234;,
 5.03985;-0.19425;-1.70234;,
 5.03985;11.13230;0.00000;,
 5.65347;11.13230;-1.57276;,
 5.03985;11.13230;-1.70234;,
 6.17366;11.13230;-1.20374;,
 6.52124;11.13230;-0.65145;,
 6.64330;11.13230;0.00000;,
 6.52124;11.13230;0.65145;,
 6.17366;11.13230;1.20374;,
 5.65347;11.13230;1.57276;,
 5.03985;11.13230;1.70234;,
 4.42624;11.13230;1.57277;,
 3.90604;11.13230;1.20374;,
 3.55846;11.13230;0.65145;,
 3.43640;11.13230;0.00000;,
 3.55846;11.13230;-0.65145;,
 3.90604;11.13230;-1.20374;,
 4.42624;11.13230;-1.57277;,
 5.03985;-0.19425;-0.00000;,
 5.03985;-0.19425;-1.70234;,
 5.65347;-0.19425;-1.57276;,
 6.17366;-0.19425;-1.20374;,
 6.52124;-0.19425;-0.65145;,
 6.64330;-0.19425;-0.00000;,
 6.52124;-0.19425;0.65145;,
 6.17366;-0.19425;1.20374;,
 5.65347;-0.19425;1.57276;,
 5.03985;-0.19425;1.70234;,
 4.42624;-0.19425;1.57277;,
 3.90604;-0.19425;1.20374;,
 3.55846;-0.19425;0.65145;,
 3.43640;-0.19425;-0.00000;,
 3.55846;-0.19425;-0.65145;,
 3.90604;-0.19425;-1.20374;,
 4.42624;-0.19425;-1.57277;;
 
 144;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;16,18,19,17;,
 4;18,20,21,19;,
 4;20,22,23,21;,
 4;22,24,25,23;,
 4;24,26,27,25;,
 4;26,28,29,27;,
 4;28,30,31,29;,
 4;30,32,33,31;,
 3;34,35,36;,
 3;34,37,35;,
 3;34,38,37;,
 3;34,39,38;,
 3;34,40,39;,
 3;34,41,40;,
 3;34,42,41;,
 3;34,43,42;,
 3;34,44,43;,
 3;34,45,44;,
 3;34,46,45;,
 3;34,47,46;,
 3;34,48,47;,
 3;34,49,48;,
 3;34,50,49;,
 3;34,36,50;,
 3;51,52,53;,
 3;51,53,54;,
 3;51,54,55;,
 3;51,55,56;,
 3;51,56,57;,
 3;51,57,58;,
 3;51,58,59;,
 3;51,59,60;,
 3;51,60,61;,
 3;51,61,62;,
 3;51,62,63;,
 3;51,63,64;,
 3;51,64,65;,
 3;51,65,66;,
 3;51,66,67;,
 3;51,67,52;,
 4;68,69,70,71;,
 4;69,72,73,70;,
 4;72,74,75,73;,
 4;74,76,77,75;,
 4;76,78,79,77;,
 4;78,80,81,79;,
 4;80,82,83,81;,
 4;82,84,85,83;,
 4;84,86,87,85;,
 4;86,88,89,87;,
 4;88,90,91,89;,
 4;90,92,93,91;,
 4;92,94,95,93;,
 4;94,96,97,95;,
 4;96,98,99,97;,
 4;98,100,101,99;,
 3;102,103,104;,
 3;102,105,103;,
 3;102,106,105;,
 3;102,107,106;,
 3;102,108,107;,
 3;102,109,108;,
 3;102,110,109;,
 3;102,111,110;,
 3;102,112,111;,
 3;102,113,112;,
 3;102,114,113;,
 3;102,115,114;,
 3;102,116,115;,
 3;102,117,116;,
 3;102,118,117;,
 3;102,104,118;,
 3;119,120,121;,
 3;119,121,122;,
 3;119,122,123;,
 3;119,123,124;,
 3;119,124,125;,
 3;119,125,126;,
 3;119,126,127;,
 3;119,127,128;,
 3;119,128,129;,
 3;119,129,130;,
 3;119,130,131;,
 3;119,131,132;,
 3;119,132,133;,
 3;119,133,134;,
 3;119,134,135;,
 3;119,135,120;,
 4;136,137,138,139;,
 4;137,140,141,138;,
 4;140,142,143,141;,
 4;142,144,145,143;,
 4;144,146,147,145;,
 4;146,148,149,147;,
 4;148,150,151,149;,
 4;150,152,153,151;,
 4;152,154,155,153;,
 4;154,156,157,155;,
 4;156,158,159,157;,
 4;158,160,161,159;,
 4;160,162,163,161;,
 4;162,164,165,163;,
 4;164,166,167,165;,
 4;166,168,169,167;,
 3;170,171,172;,
 3;170,173,171;,
 3;170,174,173;,
 3;170,175,174;,
 3;170,176,175;,
 3;170,177,176;,
 3;170,178,177;,
 3;170,179,178;,
 3;170,180,179;,
 3;170,181,180;,
 3;170,182,181;,
 3;170,183,182;,
 3;170,184,183;,
 3;170,185,184;,
 3;170,186,185;,
 3;170,172,186;,
 3;187,188,189;,
 3;187,189,190;,
 3;187,190,191;,
 3;187,191,192;,
 3;187,192,193;,
 3;187,193,194;,
 3;187,194,195;,
 3;187,195,196;,
 3;187,196,197;,
 3;187,197,198;,
 3;187,198,199;,
 3;187,199,200;,
 3;187,200,201;,
 3;187,201,202;,
 3;187,202,203;,
 3;187,203,188;;
 
 MeshMaterialList {
  3;
  144;
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
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Tree_bark.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Annual_ring.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Annual_ring.jpg";
   }
  }
 }
 MeshNormals {
  84;
  0.000000;1.000000;0.000000;,
  0.000010;0.000000;-1.000000;,
  0.400954;0.000000;-0.916098;,
  0.726374;0.000000;-0.687299;,
  0.931069;0.000000;-0.364844;,
  1.000000;0.000000;0.000000;,
  0.931069;0.000000;0.364844;,
  0.726374;0.000000;0.687299;,
  0.400954;0.000000;0.916098;,
  0.000010;0.000000;1.000000;,
  -0.400951;0.000000;0.916099;,
  -0.726379;0.000000;0.687294;,
  -0.931069;0.000000;0.364843;,
  -1.000000;0.000000;0.000000;,
  -0.931069;0.000000;-0.364843;,
  -0.726379;0.000000;-0.687294;,
  -0.400951;0.000000;-0.916099;,
  0.000000;-1.000000;-0.000000;,
  0.996375;0.085073;-0.000000;,
  0.000048;-0.001612;-0.999999;,
  0.004098;-0.510364;-0.859949;,
  0.006606;-0.822690;-0.568452;,
  0.007723;-0.961807;-0.273621;,
  0.008030;-0.999968;-0.000000;,
  0.007723;-0.961807;0.273621;,
  0.006606;-0.822690;0.568452;,
  0.004098;-0.510364;0.859949;,
  0.000048;-0.001612;0.999999;,
  -0.003761;0.505770;0.862660;,
  -0.005775;0.818972;0.573805;,
  -0.006480;0.960772;0.277262;,
  -0.006634;0.999978;0.000000;,
  -0.006480;0.960772;-0.277262;,
  -0.005775;0.818972;-0.573805;,
  -0.003761;0.505770;-0.862660;,
  -0.996469;-0.083960;0.000000;,
  0.996377;0.085048;0.000001;,
  0.996375;0.085076;0.000000;,
  0.996374;0.085081;-0.000023;,
  0.996372;0.085100;-0.000018;,
  0.996374;0.085082;0.000000;,
  0.996372;0.085100;0.000018;,
  0.996374;0.085081;0.000023;,
  0.996377;0.085048;-0.000001;,
  0.996375;0.085072;0.000008;,
  0.996376;0.085058;0.000006;,
  0.996375;0.085072;-0.000002;,
  0.996375;0.085070;0.000000;,
  0.996375;0.085072;0.000002;,
  0.996376;0.085058;-0.000006;,
  0.996375;0.085072;-0.000008;,
  -0.996455;-0.084125;0.000000;,
  -0.996373;-0.085095;-0.000012;,
  -0.996374;-0.085077;-0.000007;,
  -0.996372;-0.085107;0.000019;,
  -0.996371;-0.085113;0.000000;,
  -0.996372;-0.085107;-0.000019;,
  -0.996374;-0.085077;0.000007;,
  -0.996373;-0.085095;0.000012;,
  -0.996558;-0.082896;-0.000062;,
  -0.996575;-0.082699;-0.000111;,
  -0.996571;-0.082741;-0.000059;,
  -0.996569;-0.082764;0.000000;,
  -0.996571;-0.082741;0.000059;,
  -0.996575;-0.082699;0.000111;,
  -0.996558;-0.082896;0.000062;,
  0.000000;1.000000;0.000000;,
  0.000009;0.000000;-1.000000;,
  0.400956;0.000000;-0.916097;,
  0.726378;0.000000;-0.687295;,
  0.931067;0.000000;-0.364849;,
  1.000000;0.000000;0.000000;,
  0.931067;0.000000;0.364849;,
  0.726378;0.000000;0.687295;,
  0.400956;0.000000;0.916097;,
  0.000009;0.000000;1.000000;,
  -0.400951;0.000000;0.916100;,
  -0.726382;0.000000;0.687291;,
  -0.931068;0.000000;0.364845;,
  -1.000000;0.000000;0.000000;,
  -0.931068;0.000000;-0.364845;,
  -0.726382;0.000000;-0.687291;,
  -0.400951;0.000000;-0.916100;,
  0.000000;-1.000000;-0.000000;;
  144;
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,9,9,8;,
  4;9,10,10,9;,
  4;10,11,11,10;,
  4;11,12,12,11;,
  4;12,13,13,12;,
  4;13,14,14,13;,
  4;14,15,15,14;,
  4;15,16,16,15;,
  4;16,1,1,16;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  4;19,20,20,19;,
  4;20,21,21,20;,
  4;21,22,22,21;,
  4;22,23,23,22;,
  4;23,24,24,23;,
  4;24,25,25,24;,
  4;25,26,26,25;,
  4;26,27,27,26;,
  4;27,28,28,27;,
  4;28,29,29,28;,
  4;29,30,30,29;,
  4;30,31,31,30;,
  4;31,32,32,31;,
  4;32,33,33,32;,
  4;33,34,34,33;,
  4;34,19,19,34;,
  3;18,36,37;,
  3;18,38,36;,
  3;18,39,38;,
  3;18,40,39;,
  3;18,41,40;,
  3;18,42,41;,
  3;18,43,42;,
  3;18,37,43;,
  3;18,44,37;,
  3;18,45,44;,
  3;18,46,45;,
  3;18,47,46;,
  3;18,48,47;,
  3;18,49,48;,
  3;18,50,49;,
  3;18,37,50;,
  3;35,51,52;,
  3;35,52,53;,
  3;35,53,54;,
  3;35,54,55;,
  3;35,55,56;,
  3;35,56,57;,
  3;35,57,58;,
  3;35,58,51;,
  3;35,51,59;,
  3;35,59,60;,
  3;35,60,61;,
  3;35,61,62;,
  3;35,62,63;,
  3;35,63,64;,
  3;35,64,65;,
  3;35,65,51;,
  4;67,68,68,67;,
  4;68,69,69,68;,
  4;69,70,70,69;,
  4;70,71,71,70;,
  4;71,72,72,71;,
  4;72,73,73,72;,
  4;73,74,74,73;,
  4;74,75,75,74;,
  4;75,76,76,75;,
  4;76,77,77,76;,
  4;77,78,78,77;,
  4;78,79,79,78;,
  4;79,80,80,79;,
  4;80,81,81,80;,
  4;81,82,82,81;,
  4;82,67,67,82;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;83,83,83;,
  3;83,83,83;,
  3;83,83,83;,
  3;83,83,83;,
  3;83,83,83;,
  3;83,83,83;,
  3;83,83,83;,
  3;83,83,83;,
  3;83,83,83;,
  3;83,83,83;,
  3;83,83,83;,
  3;83,83,83;,
  3;83,83,83;,
  3;83,83,83;,
  3;83,83,83;,
  3;83,83,83;;
 }
 MeshTextureCoords {
  204;
  0.000000;0.000000;,
  0.062500;0.000000;,
  0.062500;1.000000;,
  0.000000;1.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.187500;0.000000;,
  0.187500;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.312500;0.000000;,
  0.312500;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.437500;0.000000;,
  0.437500;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.562500;0.000000;,
  0.562500;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.687500;0.000000;,
  0.687500;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.812500;0.000000;,
  0.812500;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  0.937500;0.000000;,
  0.937500;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.250796;0.495173;,
  0.281486;0.573804;,
  0.250807;0.580287;,
  0.307494;0.555351;,
  0.324870;0.527736;,
  0.330968;0.495163;,
  0.324862;0.462593;,
  0.307479;0.434981;,
  0.281467;0.416534;,
  0.250786;0.410059;,
  0.220106;0.416541;,
  0.194098;0.434995;,
  0.176722;0.462611;,
  0.170624;0.495183;,
  0.176730;0.527754;,
  0.194113;0.555365;,
  0.220125;0.573812;,
  0.250796;0.500115;,
  0.250807;0.585229;,
  0.281486;0.578746;,
  0.307494;0.560293;,
  0.324870;0.532678;,
  0.330968;0.500105;,
  0.324862;0.467535;,
  0.307479;0.439923;,
  0.281467;0.421477;,
  0.250786;0.415001;,
  0.220106;0.421484;,
  0.194098;0.439937;,
  0.176722;0.467553;,
  0.170624;0.500125;,
  0.176730;0.532696;,
  0.194113;0.560307;,
  0.220125;0.578754;,
  0.000000;0.000000;,
  0.062500;0.000000;,
  0.062500;1.000000;,
  0.000000;1.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.187500;0.000000;,
  0.187500;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.312500;0.000000;,
  0.312500;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.437500;0.000000;,
  0.437500;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.562500;0.000000;,
  0.562500;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.687500;0.000000;,
  0.687500;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.812500;0.000000;,
  0.812500;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  0.937500;0.000000;,
  0.937500;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.502506;0.620786;,
  0.558977;0.636929;,
  0.563745;0.621250;,
  0.545611;0.650151;,
  0.525683;0.658902;,
  0.502227;0.661850;,
  0.478813;0.658547;,
  0.459006;0.649495;,
  0.445822;0.636072;,
  0.441267;0.620322;,
  0.446036;0.604643;,
  0.459401;0.591421;,
  0.479329;0.582670;,
  0.502786;0.579722;,
  0.526200;0.583025;,
  0.546007;0.592077;,
  0.559191;0.605500;,
  0.495658;0.630231;,
  0.556898;0.630695;,
  0.552129;0.646374;,
  0.538763;0.659595;,
  0.518835;0.668347;,
  0.495379;0.671295;,
  0.471965;0.667992;,
  0.452158;0.658939;,
  0.438974;0.645517;,
  0.434419;0.629767;,
  0.439191;0.613707;,
  0.452560;0.600080;,
  0.472490;0.591059;,
  0.495947;0.588015;,
  0.519360;0.591414;,
  0.539165;0.600736;,
  0.552346;0.614564;,
  0.000000;0.000000;,
  0.062500;0.000000;,
  0.062500;1.000000;,
  0.000000;1.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.187500;0.000000;,
  0.187500;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.312500;0.000000;,
  0.312500;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.437500;0.000000;,
  0.437500;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.562500;0.000000;,
  0.562500;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.687500;0.000000;,
  0.687500;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.812500;0.000000;,
  0.812500;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  0.937500;0.000000;,
  0.937500;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.751993;0.495112;,
  0.782683;0.573743;,
  0.752003;0.580226;,
  0.808690;0.555290;,
  0.826066;0.527674;,
  0.832165;0.495102;,
  0.826058;0.462531;,
  0.808675;0.434920;,
  0.782664;0.416473;,
  0.751982;0.409998;,
  0.721302;0.416480;,
  0.695295;0.434934;,
  0.677919;0.462549;,
  0.671820;0.495122;,
  0.677927;0.527692;,
  0.695309;0.555304;,
  0.721321;0.573751;,
  0.751993;0.500054;,
  0.752003;0.585168;,
  0.782683;0.578685;,
  0.808690;0.560232;,
  0.826066;0.532616;,
  0.832165;0.500044;,
  0.826058;0.467473;,
  0.808675;0.439862;,
  0.782664;0.421415;,
  0.751982;0.414940;,
  0.721302;0.421422;,
  0.695295;0.439876;,
  0.677919;0.467492;,
  0.671820;0.500064;,
  0.677927;0.532634;,
  0.695309;0.560246;,
  0.721321;0.578693;;
 }
}
