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
 126;
 12.02365;13.62825;0.00000;,
 12.34467;13.77739;-0.48464;,
 12.02365;13.62825;-0.52457;,
 12.02365;13.62825;0.00000;,
 12.61679;13.90383;-0.37093;,
 12.02365;13.62825;0.00000;,
 12.79864;13.98830;-0.20074;,
 12.02365;13.62825;0.00000;,
 12.86249;14.01797;0.00000;,
 12.02365;13.62825;0.00000;,
 12.79864;13.98830;0.20074;,
 12.02365;13.62825;0.00000;,
 12.61679;13.90383;0.37093;,
 12.02365;13.62825;0.00000;,
 12.34467;13.77739;0.48464;,
 12.02365;13.62825;0.00000;,
 12.02366;13.62825;0.52457;,
 12.02365;13.62825;0.00000;,
 11.70266;13.47913;0.48464;,
 12.02365;13.62825;0.00000;,
 11.43051;13.35269;0.37093;,
 12.02365;13.62825;0.00000;,
 11.24867;13.26821;0.20074;,
 12.02365;13.62825;0.00000;,
 11.18481;13.23854;-0.00000;,
 12.02365;13.62825;0.00000;,
 11.24867;13.26821;-0.20074;,
 12.02365;13.62825;0.00000;,
 11.43051;13.35269;-0.37093;,
 12.02365;13.62825;0.00000;,
 11.70266;13.47913;-0.48464;,
 12.02365;13.62825;0.00000;,
 12.02365;13.62825;-0.52457;,
 -0.46145;-0.01283;0.00000;,
 -0.46145;-0.01283;-0.52457;,
 -0.22869;-0.07211;-0.48464;,
 -0.46145;-0.01283;0.00000;,
 -0.03135;-0.12238;-0.37093;,
 -0.46145;-0.01283;0.00000;,
 0.10050;-0.15598;-0.20074;,
 -0.46145;-0.01283;0.00000;,
 0.14682;-0.16776;0.00000;,
 -0.46145;-0.01283;0.00000;,
 0.10050;-0.15598;0.20074;,
 -0.46145;-0.01283;0.00000;,
 -0.03135;-0.12238;0.37093;,
 -0.46145;-0.01283;0.00000;,
 -0.22869;-0.07211;0.48464;,
 -0.46145;-0.01283;0.00000;,
 -0.46145;-0.01283;0.52457;,
 -0.46145;-0.01283;0.00000;,
 -0.69422;0.04646;0.48464;,
 -0.46145;-0.01283;0.00000;,
 -0.89156;0.09674;0.37093;,
 -0.46145;-0.01283;0.00000;,
 -1.02341;0.13032;0.20074;,
 -0.46145;-0.01283;0.00000;,
 -1.06971;0.14212;-0.00000;,
 -0.46145;-0.01283;0.00000;,
 -1.02341;0.13032;-0.20074;,
 -0.46145;-0.01283;0.00000;,
 -0.89156;0.09674;-0.37093;,
 -0.46145;-0.01283;0.00000;,
 -0.69422;0.04646;-0.48464;,
 -0.46145;-0.01283;0.00000;,
 -0.46145;-0.01283;-0.52457;,
 2.34038;3.54361;-0.48464;,
 5.44605;6.92956;-0.48464;,
 5.67881;6.87027;-0.52457;,
 2.57314;3.48431;-0.52457;,
 2.14305;3.59387;-0.37093;,
 5.24871;6.97983;-0.37093;,
 2.57314;3.48431;-0.52457;,
 5.67881;6.87027;-0.52457;,
 5.91159;6.81097;-0.48464;,
 2.80592;3.42501;-0.48464;,
 2.01117;3.62745;-0.20074;,
 5.11686;7.01342;-0.20074;,
 6.10892;6.76070;-0.37093;,
 3.00326;3.37474;-0.37093;,
 1.96489;3.63926;-0.00000;,
 5.07057;7.02521;-0.00000;,
 6.24077;6.72710;-0.20074;,
 3.13511;3.34115;-0.20074;,
 2.01117;3.62745;0.20074;,
 5.11686;7.01342;0.20074;,
 6.28708;6.71531;0.00000;,
 3.18140;3.32935;0.00000;,
 2.14305;3.59387;0.37093;,
 5.24871;6.97983;0.37093;,
 6.24077;6.72710;0.20074;,
 3.13511;3.34115;0.20074;,
 2.34038;3.54361;0.48464;,
 5.44605;6.92956;0.48464;,
 6.10892;6.76070;0.37093;,
 3.00326;3.37474;0.37093;,
 2.57315;3.48431;0.52457;,
 5.67881;6.87027;0.52457;,
 5.91159;6.81097;0.48464;,
 2.80592;3.42501;0.48464;,
 8.49077;10.22311;-0.48464;,
 8.72353;10.16380;-0.52457;,
 8.29343;10.27338;-0.37093;,
 8.72353;10.16380;-0.52457;,
 8.95631;10.10451;-0.48464;,
 8.16157;10.30695;-0.20074;,
 9.15363;10.05425;-0.37093;,
 8.11527;10.31876;-0.00000;,
 9.28549;10.02065;-0.20074;,
 8.16157;10.30695;0.20074;,
 9.33177;10.00887;0.00000;,
 8.29343;10.27338;0.37093;,
 9.28549;10.02065;0.20074;,
 8.49077;10.22311;0.48464;,
 9.15363;10.05425;0.37093;,
 8.72352;10.16380;0.52457;,
 8.95631;10.10451;0.48464;,
 30.50772;19.97177;-0.13597;,
 45.79830;22.01007;-14.27811;,
 32.09176;17.90739;-20.13597;,
 51.16484;24.11450;-0.13597;,
 46.39150;21.23701;14.00616;,
 18.18846;14.06129;14.00616;,
 33.57644;15.97250;19.86403;,
 11.54130;13.62571;-0.13597;,
 17.95252;14.36879;-14.27811;;
 
 103;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 3;17,18,16;,
 3;19,20,18;,
 3;21,22,20;,
 3;23,24,22;,
 3;25,26,24;,
 3;27,28,26;,
 3;29,30,28;,
 3;31,32,30;,
 3;33,34,35;,
 3;36,35,37;,
 3;38,37,39;,
 3;40,39,41;,
 3;42,41,43;,
 3;44,43,45;,
 3;46,45,47;,
 3;48,47,49;,
 3;50,49,51;,
 3;52,51,53;,
 3;54,53,55;,
 3;56,55,57;,
 3;58,57,59;,
 3;60,59,61;,
 3;62,61,63;,
 3;64,63,65;,
 4;66,67,68,69;,
 4;66,69,65,63;,
 4;66,63,61,70;,
 4;66,70,71,67;,
 4;72,73,74,75;,
 4;72,75,35,34;,
 4;70,61,59,76;,
 4;70,76,77,71;,
 4;75,74,78,79;,
 4;75,79,37,35;,
 4;76,59,57,80;,
 4;76,80,81,77;,
 4;79,78,82,83;,
 4;79,83,39,37;,
 4;80,57,55,84;,
 4;80,84,85,81;,
 4;83,82,86,87;,
 4;83,87,41,39;,
 4;84,55,53,88;,
 4;84,88,89,85;,
 4;87,86,90,91;,
 4;87,91,43,41;,
 4;88,53,51,92;,
 4;88,92,93,89;,
 4;91,90,94,95;,
 4;91,95,45,43;,
 4;92,51,49,96;,
 4;92,96,97,93;,
 4;95,94,98,99;,
 4;95,99,47,45;,
 4;96,49,47,99;,
 4;96,99,98,97;,
 4;100,30,32,101;,
 4;100,101,68,67;,
 4;100,67,71,102;,
 4;100,102,28,30;,
 4;103,2,1,104;,
 4;103,104,74,73;,
 4;102,71,77,105;,
 4;102,105,26,28;,
 4;104,1,4,106;,
 4;104,106,78,74;,
 4;105,77,81,107;,
 4;105,107,24,26;,
 4;106,4,6,108;,
 4;106,108,82,78;,
 4;107,81,85,109;,
 4;107,109,22,24;,
 4;108,6,8,110;,
 4;108,110,86,82;,
 4;109,85,89,111;,
 4;109,111,20,22;,
 4;110,8,10,112;,
 4;110,112,90,86;,
 4;111,89,93,113;,
 4;111,113,18,20;,
 4;112,10,12,114;,
 4;112,114,94,90;,
 4;113,93,97,115;,
 4;113,115,16,18;,
 4;114,12,14,116;,
 4;114,116,98,94;,
 4;115,97,98,116;,
 4;115,116,14,16;,
 3;117,118,119;,
 3;117,120,118;,
 3;117,121,120;,
 3;117,122,123;,
 3;117,124,122;,
 3;117,125,124;,
 3;117,119,125;;
 
 MeshMaterialList {
  2;
  103;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
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
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\dry_leaf.jpg";
   }
  }
  Material {
   0.680784;0.301176;0.185098;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  117;
  -0.421336;0.906905;-0.000001;,
  -0.421327;0.906909;0.000000;,
  -0.421357;0.906895;-0.000010;,
  -0.421350;0.906898;0.000018;,
  -0.421337;0.906904;0.000023;,
  -0.421344;0.906901;0.000000;,
  -0.421337;0.906904;-0.000023;,
  -0.421350;0.906898;-0.000018;,
  -0.421353;0.906897;0.000008;,
  -0.421327;0.906909;-0.000006;,
  -0.476772;0.833175;0.280194;,
  -0.518950;0.787091;0.333436;,
  -0.553306;0.817302;0.160840;,
  -0.562745;0.826631;0.000000;,
  -0.553306;0.817302;-0.160840;,
  -0.518950;0.787091;-0.333436;,
  -0.476769;0.833175;-0.280196;,
  -0.246817;-0.969062;0.000000;,
  -0.246825;-0.969060;-0.000010;,
  -0.246871;-0.969049;-0.000052;,
  -0.246866;-0.969050;0.000008;,
  -0.246835;-0.969058;-0.000000;,
  -0.246866;-0.969050;-0.000008;,
  -0.246871;-0.969049;0.000052;,
  -0.246825;-0.969060;0.000010;,
  -0.246857;-0.969052;-0.000014;,
  -0.246860;-0.969051;0.000002;,
  -0.246843;-0.969056;0.000011;,
  -0.246850;-0.969054;-0.000000;,
  -0.246843;-0.969055;-0.000011;,
  -0.246860;-0.969051;-0.000002;,
  -0.246857;-0.969052;0.000014;,
  -0.246848;-0.969054;-0.000000;,
  -0.507598;0.467412;-0.723789;,
  -0.270160;0.248772;-0.930122;,
  -0.674710;0.621293;-0.398448;,
  -0.000003;0.000003;-1.000000;,
  -0.735627;0.677387;0.000000;,
  0.270158;-0.248770;-0.930123;,
  -0.674710;0.621293;0.398448;,
  0.507594;-0.467407;-0.723794;,
  -0.507598;0.467412;0.723789;,
  0.674712;-0.621293;-0.398446;,
  -0.270160;0.248772;0.930122;,
  0.735628;-0.677386;0.000000;,
  -0.000004;0.000003;1.000000;,
  0.674712;-0.621293;0.398446;,
  0.270158;-0.248770;0.930123;,
  0.507594;-0.467407;0.723794;,
  -0.272196;0.242884;-0.931083;,
  -0.000002;0.000002;-1.000000;,
  -0.512785;0.457539;-0.726436;,
  0.272191;-0.242881;-0.931086;,
  -0.683554;0.609872;-0.401012;,
  0.512787;-0.457542;-0.726433;,
  -0.746187;0.665736;0.000000;,
  0.683552;-0.609871;-0.401017;,
  -0.683554;0.609872;0.401012;,
  0.746187;-0.665737;0.000000;,
  -0.512785;0.457539;0.726436;,
  0.683552;-0.609871;0.401017;,
  -0.272196;0.242884;0.931084;,
  0.512787;-0.457542;0.726433;,
  -0.000002;0.000001;1.000000;,
  0.272192;-0.242881;0.931085;,
  -0.297102;0.285334;-0.911216;,
  -0.000464;0.001829;-0.999998;,
  -0.527838;0.513361;-0.676644;,
  0.296269;-0.272579;-0.915383;,
  -0.666726;0.654164;-0.357137;,
  0.534528;-0.487095;-0.690665;,
  -0.712764;0.701404;0.000000;,
  0.688332;-0.623237;-0.371180;,
  -0.666726;0.654164;0.357137;,
  0.742014;-0.670385;0.000000;,
  -0.527837;0.513361;0.676645;,
  0.688332;-0.623237;0.371180;,
  -0.297103;0.285335;0.911215;,
  0.534528;-0.487095;0.690665;,
  -0.000466;0.001831;0.999998;,
  0.296268;-0.272578;0.915383;,
  -0.000001;0.000000;-1.000000;,
  -0.273963;0.237729;-0.931895;,
  -0.517270;0.448856;-0.728670;,
  0.273961;-0.237728;-0.931896;,
  -0.691178;0.599761;-0.403186;,
  0.517272;-0.448860;-0.728666;,
  -0.755290;0.655391;0.000000;,
  0.691175;-0.599761;-0.403193;,
  -0.691178;0.599761;0.403186;,
  0.755287;-0.655394;0.000000;,
  -0.517270;0.448856;0.728670;,
  0.691175;-0.599761;0.403193;,
  -0.273963;0.237729;0.931896;,
  0.517272;-0.448860;0.728666;,
  -0.000000;-0.000000;1.000000;,
  0.273961;-0.237729;0.931896;,
  -0.174193;0.168214;-0.970238;,
  -0.000931;0.003679;-0.999993;,
  0.322338;-0.295368;-0.899364;,
  0.560914;-0.504466;-0.656422;,
  0.702345;-0.623181;-0.344031;,
  0.749581;-0.661913;0.000000;,
  0.702345;-0.623181;0.344031;,
  0.560914;-0.504466;0.656422;,
  -0.000933;0.003681;0.999993;,
  -0.174195;0.168217;0.970237;,
  0.322337;-0.295368;0.899364;,
  -0.252679;0.967499;0.009954;,
  -0.262992;0.957351;-0.119646;,
  -0.217168;0.971522;-0.094779;,
  -0.196540;0.980019;0.030588;,
  -0.194913;0.971903;0.131964;,
  -0.202907;0.953736;0.221846;,
  -0.259734;0.950427;0.170960;,
  -0.317278;0.948246;0.012809;,
  -0.301930;0.947278;-0.107253;;
  103;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,7,6;,
  3;0,8,7;,
  3;0,9,8;,
  3;0,10,9;,
  3;0,11,10;,
  3;0,12,11;,
  3;0,13,12;,
  3;0,14,13;,
  3;0,15,14;,
  3;0,16,15;,
  3;0,1,16;,
  3;32,17,18;,
  3;32,18,19;,
  3;32,19,20;,
  3;32,20,21;,
  3;32,21,22;,
  3;32,22,23;,
  3;32,23,24;,
  3;32,24,17;,
  3;32,17,25;,
  3;32,25,26;,
  3;32,26,27;,
  3;32,27,28;,
  3;32,28,29;,
  3;32,29,30;,
  3;32,30,31;,
  3;32,31,17;,
  4;49,34,36,50;,
  4;49,50,81,82;,
  4;49,82,83,51;,
  4;49,51,33,34;,
  4;50,36,38,52;,
  4;50,52,84,81;,
  4;51,83,85,53;,
  4;51,53,35,33;,
  4;52,38,40,54;,
  4;52,54,86,84;,
  4;53,85,87,55;,
  4;53,55,37,35;,
  4;54,40,42,56;,
  4;54,56,88,86;,
  4;55,87,89,57;,
  4;55,57,39,37;,
  4;56,42,44,58;,
  4;56,58,90,88;,
  4;57,89,91,59;,
  4;57,59,41,39;,
  4;58,44,46,60;,
  4;58,60,92,90;,
  4;59,91,93,61;,
  4;59,61,43,41;,
  4;60,46,48,62;,
  4;60,62,94,92;,
  4;61,93,95,63;,
  4;61,63,45,43;,
  4;62,48,47,64;,
  4;62,64,96,94;,
  4;63,95,96,64;,
  4;63,64,47,45;,
  4;65,97,98,66;,
  4;65,66,36,34;,
  4;65,34,33,67;,
  4;65,67,15,16;,
  4;66,98,99,68;,
  4;66,68,38,36;,
  4;67,33,35,69;,
  4;67,69,14,15;,
  4;68,99,100,70;,
  4;68,70,40,38;,
  4;69,35,37,71;,
  4;69,71,13,14;,
  4;70,100,101,72;,
  4;70,72,42,40;,
  4;71,37,39,73;,
  4;71,73,12,13;,
  4;72,101,102,74;,
  4;72,74,44,42;,
  4;73,39,41,75;,
  4;73,75,11,12;,
  4;74,102,103,76;,
  4;74,76,46,44;,
  4;75,41,43,77;,
  4;75,77,10,11;,
  4;76,103,104,78;,
  4;76,78,48,46;,
  4;77,43,45,79;,
  4;77,79,105,106;,
  4;78,104,107,80;,
  4;78,80,47,48;,
  4;79,45,47,80;,
  4;79,80,107,105;,
  3;108,110,109;,
  3;108,111,110;,
  3;108,112,111;,
  3;108,114,113;,
  3;108,115,114;,
  3;108,116,115;,
  3;108,109,116;;
 }
 MeshTextureCoords {
  126;
  0.031250;0.000000;,
  0.062500;0.000000;,
  0.000000;0.000000;,
  0.093750;0.000000;,
  0.125000;0.000000;,
  0.156250;0.000000;,
  0.187500;0.000000;,
  0.218750;0.000000;,
  0.250000;0.000000;,
  0.281250;0.000000;,
  0.312500;0.000000;,
  0.343750;0.000000;,
  0.375000;0.000000;,
  0.406250;0.000000;,
  0.437500;0.000000;,
  0.468750;0.000000;,
  0.500000;0.000000;,
  0.531250;0.000000;,
  0.562500;0.000000;,
  0.593750;0.000000;,
  0.625000;0.000000;,
  0.656250;0.000000;,
  0.687500;0.000000;,
  0.718750;0.000000;,
  0.750000;0.000000;,
  0.781250;0.000000;,
  0.812500;0.000000;,
  0.843750;0.000000;,
  0.875000;0.000000;,
  0.906250;0.000000;,
  0.937500;0.000000;,
  0.968750;0.000000;,
  1.000000;0.000000;,
  0.031250;1.000000;,
  0.000000;1.000000;,
  0.062500;1.000000;,
  0.093750;1.000000;,
  0.125000;1.000000;,
  0.156250;1.000000;,
  0.187500;1.000000;,
  0.218750;1.000000;,
  0.250000;1.000000;,
  0.281250;1.000000;,
  0.312500;1.000000;,
  0.343750;1.000000;,
  0.375000;1.000000;,
  0.406250;1.000000;,
  0.437500;1.000000;,
  0.468750;1.000000;,
  0.500000;1.000000;,
  0.531250;1.000000;,
  0.562500;1.000000;,
  0.593750;1.000000;,
  0.625000;1.000000;,
  0.656250;1.000000;,
  0.687500;1.000000;,
  0.718750;1.000000;,
  0.750000;1.000000;,
  0.781250;1.000000;,
  0.812500;1.000000;,
  0.843750;1.000000;,
  0.875000;1.000000;,
  0.906250;1.000000;,
  0.937500;1.000000;,
  0.968750;1.000000;,
  1.000000;1.000000;,
  0.937500;0.746149;,
  0.937500;0.497830;,
  1.000000;0.497830;,
  1.000000;0.746149;,
  0.875000;0.746149;,
  0.875000;0.497830;,
  0.000000;0.746149;,
  0.000000;0.497830;,
  0.062500;0.497830;,
  0.062500;0.746149;,
  0.812500;0.746149;,
  0.812500;0.497830;,
  0.125000;0.497830;,
  0.125000;0.746149;,
  0.750000;0.746149;,
  0.750000;0.497830;,
  0.187500;0.497830;,
  0.187500;0.746149;,
  0.687500;0.746149;,
  0.687500;0.497830;,
  0.250000;0.497830;,
  0.250000;0.746149;,
  0.625000;0.746149;,
  0.625000;0.497830;,
  0.312500;0.497830;,
  0.312500;0.746149;,
  0.562500;0.746149;,
  0.562500;0.497830;,
  0.375000;0.497830;,
  0.375000;0.746149;,
  0.500000;0.746149;,
  0.500000;0.497830;,
  0.437500;0.497830;,
  0.437500;0.746149;,
  0.937500;0.255929;,
  1.000000;0.255929;,
  0.875000;0.255929;,
  0.000000;0.255929;,
  0.062500;0.255929;,
  0.812500;0.255929;,
  0.125000;0.255929;,
  0.750000;0.255929;,
  0.187500;0.255929;,
  0.687500;0.255929;,
  0.250000;0.255929;,
  0.625000;0.255929;,
  0.312500;0.255929;,
  0.562500;0.255929;,
  0.375000;0.255929;,
  0.500000;0.255929;,
  0.437500;0.255929;,
  0.546371;0.469777;,
  0.925543;0.113844;,
  0.581599;-0.031375;,
  1.062795;0.467077;,
  0.946540;0.820349;,
  0.241464;0.823371;,
  0.627434;0.967266;,
  0.072204;0.471145;,
  0.229396;0.116585;;
 }
}
