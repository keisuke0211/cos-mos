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
 -5.08586;16.10027;-1.61952;,
 -4.45989;16.10027;-1.49624;,
 -4.45989;-0.11987;-1.49624;,
 -5.08586;-0.11987;-1.61952;,
 -3.92921;16.10027;-1.14518;,
 -3.92921;-0.11987;-1.14518;,
 -3.57462;16.10027;-0.61976;,
 -3.57462;-0.11987;-0.61976;,
 -3.45011;16.10027;0.00000;,
 -3.45011;-0.11987;0.00000;,
 -3.57462;16.10027;0.61976;,
 -3.57462;-0.11987;0.61976;,
 -3.92921;16.10027;1.14518;,
 -3.92921;-0.11987;1.14518;,
 -4.45989;16.10027;1.49624;,
 -4.45989;-0.11987;1.49624;,
 -5.08586;16.10027;1.61952;,
 -5.08586;-0.11987;1.61952;,
 -5.71184;16.10027;1.49625;,
 -5.71184;-0.11987;1.49625;,
 -6.24252;16.10027;1.14518;,
 -6.24252;-0.11987;1.14518;,
 -6.59711;16.10027;0.61976;,
 -6.59711;-0.11987;0.61976;,
 -6.72162;16.10027;-0.00000;,
 -6.72162;-0.11987;-0.00000;,
 -6.59711;16.10027;-0.61976;,
 -6.59711;-0.11987;-0.61976;,
 -6.24252;16.10027;-1.14518;,
 -6.24252;-0.11987;-1.14518;,
 -5.71184;16.10027;-1.49625;,
 -5.71184;-0.11987;-1.49625;,
 -5.08586;16.10027;-1.61952;,
 -5.08586;-0.11987;-1.61952;,
 -5.08586;16.10027;0.00000;,
 -4.45989;16.10027;-1.49624;,
 -5.08586;16.10027;-1.61952;,
 -3.92921;16.10027;-1.14518;,
 -3.57462;16.10027;-0.61976;,
 -3.45011;16.10027;0.00000;,
 -3.57462;16.10027;0.61976;,
 -3.92921;16.10027;1.14518;,
 -4.45989;16.10027;1.49624;,
 -5.08586;16.10027;1.61952;,
 -5.71184;16.10027;1.49625;,
 -6.24252;16.10027;1.14518;,
 -6.59711;16.10027;0.61976;,
 -6.72162;16.10027;-0.00000;,
 -6.59711;16.10027;-0.61976;,
 -6.24252;16.10027;-1.14518;,
 -5.71184;16.10027;-1.49625;,
 -5.08586;-0.11987;0.00000;,
 -5.08586;-0.11987;-1.61952;,
 -4.45989;-0.11987;-1.49624;,
 -3.92921;-0.11987;-1.14518;,
 -3.57462;-0.11987;-0.61976;,
 -3.45011;-0.11987;0.00000;,
 -3.57462;-0.11987;0.61976;,
 -3.92921;-0.11987;1.14518;,
 -4.45989;-0.11987;1.49624;,
 -5.08586;-0.11987;1.61952;,
 -5.71184;-0.11987;1.49625;,
 -6.24252;-0.11987;1.14518;,
 -6.59711;-0.11987;0.61976;,
 -6.72162;-0.11987;-0.00000;,
 -6.59711;-0.11987;-0.61976;,
 -6.24252;-0.11987;-1.14518;,
 -5.71184;-0.11987;-1.49625;,
 8.13394;8.34356;-1.16527;,
 8.16132;7.89331;-1.07657;,
 -7.97257;7.71145;-1.07657;,
 -7.99997;8.16169;-1.16527;,
 8.18453;7.51161;-0.82397;,
 -7.94936;7.32975;-0.82397;,
 8.20007;7.25656;-0.44593;,
 -7.93384;7.07470;-0.44593;,
 8.20551;7.16701;0.00000;,
 -7.92838;6.98514;0.00000;,
 8.20007;7.25656;0.44593;,
 -7.93384;7.07470;0.44593;,
 8.18453;7.51161;0.82397;,
 -7.94936;7.32975;0.82397;,
 8.16132;7.89331;1.07657;,
 -7.97257;7.71145;1.07657;,
 8.13394;8.34356;1.16527;,
 -7.99997;8.16169;1.16527;,
 8.10655;8.79380;1.07657;,
 -8.02737;8.62285;1.07657;,
 8.08334;9.17550;0.82397;,
 -8.05058;9.01615;0.82397;,
 8.06783;9.43054;0.44593;,
 -8.06608;9.27894;0.44593;,
 8.06238;9.52010;-0.00000;,
 -8.07153;9.37122;-0.00000;,
 8.06783;9.43054;-0.44593;,
 -8.06608;9.27894;-0.44593;,
 8.08334;9.17550;-0.82397;,
 -8.05058;9.01615;-0.82397;,
 8.10655;8.79380;-1.07657;,
 -8.02737;8.62285;-1.07657;,
 8.13394;8.34356;-1.16527;,
 -7.99997;8.16169;-1.16527;,
 8.13394;8.34356;0.00000;,
 8.16132;7.89331;-1.07657;,
 8.13394;8.34356;-1.16527;,
 8.18453;7.51161;-0.82397;,
 8.20007;7.25656;-0.44593;,
 8.20551;7.16701;0.00000;,
 8.20007;7.25656;0.44593;,
 8.18453;7.51161;0.82397;,
 8.16132;7.89331;1.07657;,
 8.13394;8.34356;1.16527;,
 8.10655;8.79380;1.07657;,
 8.08334;9.17550;0.82397;,
 8.06783;9.43054;0.44593;,
 8.06238;9.52010;-0.00000;,
 8.06783;9.43054;-0.44593;,
 8.08334;9.17550;-0.82397;,
 8.10655;8.79380;-1.07657;,
 -7.99997;8.16169;0.00000;,
 -7.99997;8.16169;-1.16527;,
 -7.97257;7.71145;-1.07657;,
 -7.94936;7.32975;-0.82397;,
 -7.93384;7.07470;-0.44593;,
 -7.92838;6.98514;0.00000;,
 -7.93384;7.07470;0.44593;,
 -7.94936;7.32975;0.82397;,
 -7.97257;7.71145;1.07657;,
 -7.99997;8.16169;1.16527;,
 -8.02737;8.62285;1.07657;,
 -8.05058;9.01615;0.82397;,
 -8.06608;9.27894;0.44593;,
 -8.07153;9.37122;-0.00000;,
 -8.06608;9.27894;-0.44593;,
 -8.05058;9.01615;-0.82397;,
 -8.02737;8.62285;-1.07657;,
 5.14006;16.10027;-1.61952;,
 5.76604;16.10027;-1.49624;,
 5.76604;-0.11987;-1.49624;,
 5.14006;-0.11987;-1.61952;,
 6.29671;16.10027;-1.14518;,
 6.29671;-0.11987;-1.14518;,
 6.65130;16.10027;-0.61976;,
 6.65130;-0.11987;-0.61976;,
 6.77582;16.10027;0.00000;,
 6.77582;-0.11987;0.00000;,
 6.65130;16.10027;0.61976;,
 6.65130;-0.11987;0.61976;,
 6.29671;16.10027;1.14518;,
 6.29671;-0.11987;1.14518;,
 5.76604;16.10027;1.49624;,
 5.76604;-0.11987;1.49624;,
 5.14006;16.10027;1.61952;,
 5.14006;-0.11987;1.61952;,
 4.51408;16.10027;1.49625;,
 4.51408;-0.11987;1.49625;,
 3.98340;16.10027;1.14518;,
 3.98340;-0.11987;1.14518;,
 3.62882;16.10027;0.61976;,
 3.62882;-0.11987;0.61976;,
 3.50430;16.10027;-0.00000;,
 3.50430;-0.11987;-0.00000;,
 3.62882;16.10027;-0.61976;,
 3.62882;-0.11987;-0.61976;,
 3.98340;16.10027;-1.14518;,
 3.98340;-0.11987;-1.14518;,
 4.51408;16.10027;-1.49625;,
 4.51408;-0.11987;-1.49625;,
 5.14006;16.10027;-1.61952;,
 5.14006;-0.11987;-1.61952;,
 5.14006;16.10027;0.00000;,
 5.76604;16.10027;-1.49624;,
 5.14006;16.10027;-1.61952;,
 6.29671;16.10027;-1.14518;,
 6.65130;16.10027;-0.61976;,
 6.77582;16.10027;0.00000;,
 6.65130;16.10027;0.61976;,
 6.29671;16.10027;1.14518;,
 5.76604;16.10027;1.49624;,
 5.14006;16.10027;1.61952;,
 4.51408;16.10027;1.49625;,
 3.98340;16.10027;1.14518;,
 3.62882;16.10027;0.61976;,
 3.50430;16.10027;-0.00000;,
 3.62882;16.10027;-0.61976;,
 3.98340;16.10027;-1.14518;,
 4.51408;16.10027;-1.49625;,
 5.14006;-0.11987;-0.00000;,
 5.14006;-0.11987;-1.61952;,
 5.76604;-0.11987;-1.49624;,
 6.29671;-0.11987;-1.14518;,
 6.65130;-0.11987;-0.61976;,
 6.77582;-0.11987;0.00000;,
 6.65130;-0.11987;0.61976;,
 6.29671;-0.11987;1.14518;,
 5.76604;-0.11987;1.49624;,
 5.14006;-0.11987;1.61952;,
 4.51408;-0.11987;1.49625;,
 3.98340;-0.11987;1.14518;,
 3.62882;-0.11987;0.61976;,
 3.50430;-0.11987;-0.00000;,
 3.62882;-0.11987;-0.61976;,
 3.98340;-0.11987;-1.14518;,
 4.51408;-0.11987;-1.49625;;
 
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
  0.000000;0.000000;-1.000000;,
  0.379679;0.000000;-0.925118;,
  0.703841;0.000000;-0.710357;,
  0.922624;0.000000;-0.385700;,
  1.000000;0.000000;0.000000;,
  0.922624;0.000000;0.385700;,
  0.703841;0.000000;0.710358;,
  0.379679;0.000000;0.925118;,
  0.000001;0.000000;1.000000;,
  -0.379679;0.000000;0.925118;,
  -0.703841;0.000000;0.710357;,
  -0.922625;0.000000;0.385699;,
  -1.000000;0.000000;-0.000000;,
  -0.922625;0.000000;-0.385699;,
  -0.703841;0.000000;-0.710357;,
  -0.379679;0.000000;-0.925118;,
  0.000000;-1.000000;-0.000000;,
  0.998155;0.060713;0.000000;,
  0.000046;-0.001120;-0.999999;,
  0.004279;-0.379571;-0.925152;,
  0.007932;-0.703705;-0.710448;,
  0.010399;-0.922531;-0.385783;,
  0.011271;-0.999937;-0.000000;,
  0.010399;-0.922531;0.385783;,
  0.007932;-0.703705;0.710449;,
  0.004279;-0.379572;0.925153;,
  0.000046;-0.001120;0.999999;,
  -0.003911;0.375382;0.926862;,
  -0.006903;0.698814;0.715270;,
  -0.008710;0.920638;0.390319;,
  -0.009312;0.999957;-0.000000;,
  -0.008710;0.920638;-0.390319;,
  -0.006903;0.698814;-0.715270;,
  -0.003911;0.375382;-0.926862;,
  -0.998204;-0.059912;0.000000;,
  0.998156;0.060707;0.000002;,
  0.998155;0.060710;0.000000;,
  0.998155;0.060723;-0.000017;,
  0.998154;0.060732;-0.000008;,
  0.998155;0.060719;0.000000;,
  0.998154;0.060732;0.000008;,
  0.998155;0.060723;0.000017;,
  0.998156;0.060707;-0.000002;,
  0.998155;0.060709;-0.000000;,
  0.998156;0.060708;0.000002;,
  0.998156;0.060704;0.000006;,
  0.998156;0.060704;-0.000000;,
  0.998156;0.060704;-0.000006;,
  0.998156;0.060708;-0.000002;,
  0.998155;0.060709;0.000000;,
  -0.998197;-0.060023;0.000000;,
  -0.998155;-0.060723;-0.000006;,
  -0.998155;-0.060712;-0.000008;,
  -0.998155;-0.060723;0.000011;,
  -0.998154;-0.060729;0.000000;,
  -0.998155;-0.060723;-0.000011;,
  -0.998155;-0.060712;0.000008;,
  -0.998155;-0.060723;0.000006;,
  -0.998249;-0.059153;-0.000067;,
  -0.998257;-0.059016;-0.000114;,
  -0.998255;-0.059049;-0.000060;,
  -0.998254;-0.059062;0.000000;,
  -0.998255;-0.059049;0.000060;,
  -0.998257;-0.059016;0.000114;,
  -0.998249;-0.059153;0.000067;,
  0.000000;1.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.379679;0.000000;-0.925118;,
  0.703841;0.000000;-0.710357;,
  0.922624;0.000000;-0.385700;,
  1.000000;0.000000;0.000000;,
  0.922624;0.000000;0.385700;,
  0.703841;0.000000;0.710358;,
  0.379679;0.000000;0.925118;,
  0.000001;0.000000;1.000000;,
  -0.379678;0.000000;0.925119;,
  -0.703841;0.000000;0.710358;,
  -0.922625;0.000000;0.385699;,
  -1.000000;0.000000;-0.000000;,
  -0.922625;0.000000;-0.385699;,
  -0.703841;0.000000;-0.710358;,
  -0.379678;0.000000;-0.925119;,
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
  0.245707;0.493005;,
  0.277015;0.567810;,
  0.245717;0.573978;,
  0.303546;0.550254;,
  0.321273;0.523983;,
  0.327495;0.492995;,
  0.321265;0.462009;,
  0.303533;0.435741;,
  0.276997;0.418192;,
  0.245697;0.412032;,
  0.214399;0.418199;,
  0.187867;0.435755;,
  0.170141;0.462027;,
  0.163919;0.493015;,
  0.170148;0.524001;,
  0.187881;0.550268;,
  0.214417;0.567818;,
  0.245707;0.500083;,
  0.245717;0.581056;,
  0.277015;0.574889;,
  0.303546;0.557333;,
  0.321273;0.531061;,
  0.327495;0.500073;,
  0.321265;0.469087;,
  0.303533;0.442819;,
  0.276997;0.425270;,
  0.245697;0.419110;,
  0.214399;0.425277;,
  0.187867;0.442833;,
  0.170141;0.469105;,
  0.163919;0.500093;,
  0.170148;0.531079;,
  0.187881;0.557347;,
  0.214417;0.574896;,
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
  0.503567;0.489374;,
  0.557234;0.512288;,
  0.561826;0.489815;,
  0.544471;0.531272;,
  0.525480;0.543878;,
  0.503154;0.548185;,
  0.480890;0.543540;,
  0.462079;0.530648;,
  0.449584;0.511472;,
  0.445307;0.488933;,
  0.449900;0.466460;,
  0.462663;0.447476;,
  0.481653;0.434871;,
  0.503979;0.430563;,
  0.526243;0.435208;,
  0.545054;0.448100;,
  0.557550;0.467276;,
  0.496559;0.501621;,
  0.554818;0.502063;,
  0.550226;0.524535;,
  0.537463;0.543519;,
  0.518472;0.556125;,
  0.496146;0.560433;,
  0.473882;0.555787;,
  0.455071;0.542895;,
  0.442576;0.523720;,
  0.438299;0.501180;,
  0.442896;0.478162;,
  0.455663;0.458598;,
  0.474656;0.445605;,
  0.496984;0.441161;,
  0.519247;0.445943;,
  0.538055;0.459222;,
  0.550546;0.478977;,
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
  0.757003;0.492943;,
  0.788311;0.567749;,
  0.757013;0.573916;,
  0.814843;0.550193;,
  0.832569;0.523921;,
  0.838791;0.492933;,
  0.832561;0.461947;,
  0.814829;0.435679;,
  0.788293;0.418130;,
  0.756993;0.411970;,
  0.725695;0.418137;,
  0.699163;0.435693;,
  0.681437;0.461965;,
  0.675215;0.492953;,
  0.681445;0.523939;,
  0.699177;0.550207;,
  0.725713;0.567756;,
  0.757003;0.500021;,
  0.757013;0.580994;,
  0.788311;0.574827;,
  0.814843;0.557271;,
  0.832569;0.530999;,
  0.838791;0.500011;,
  0.832561;0.469025;,
  0.814829;0.442758;,
  0.788293;0.425208;,
  0.756993;0.419048;,
  0.725695;0.425216;,
  0.699163;0.442772;,
  0.681437;0.469043;,
  0.675215;0.500031;,
  0.681445;0.531017;,
  0.699177;0.557285;,
  0.725713;0.574834;;
 }
}