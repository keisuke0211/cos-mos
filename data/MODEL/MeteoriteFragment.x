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
 79;
 -0.95262;2.59356;0.60209;,
 -2.35012;1.84044;0.28641;,
 -2.48016;1.57333;1.31869;,
 -0.95262;2.59356;0.60209;,
 -1.39491;2.42429;-0.57901;,
 -0.95262;2.59356;0.60209;,
 -0.03905;2.96582;-0.77667;,
 -0.95262;2.59356;0.60209;,
 0.82864;3.16173;-0.19375;,
 -0.95262;2.59356;0.60209;,
 0.69989;2.89727;0.82828;,
 -0.95262;2.59356;0.60209;,
 -0.34989;2.32736;1.69073;,
 -0.95262;2.59356;0.60209;,
 -1.60377;1.77121;1.90745;,
 -0.95262;2.59356;0.60209;,
 -2.48016;1.57333;1.31869;,
 -3.23348;0.58994;-0.07529;,
 -3.44455;0.15642;1.60013;,
 -1.60941;1.84532;-1.67277;,
 0.89588;2.84592;-2.03800;,
 1.82609;2.19719;-0.75829;,
 2.06380;2.06479;0.44925;,
 0.42246;1.12558;1.93739;,
 -2.02214;0.47759;2.55571;,
 -3.44455;0.15642;1.60013;,
 -3.19730;-0.19087;-0.32129;,
 -3.15679;-0.52068;1.31226;,
 -1.37160;0.63202;-2.22320;,
 1.67681;1.73696;-2.29998;,
 2.58031;1.91825;-1.11745;,
 2.40072;1.54939;0.30807;,
 1.24007;0.55436;2.60115;,
 -1.79943;-0.21421;2.22413;,
 -3.15679;-0.52068;1.31226;,
 -2.89669;-0.88348;-0.47226;,
 -3.11470;-1.33126;1.25827;,
 -1.10733;-0.32577;-2.62767;,
 1.69883;1.08038;-2.21792;,
 3.00880;1.37616;-1.33788;,
 2.53890;0.97778;0.12236;,
 1.17970;-0.12452;2.62198;,
 -1.64551;-0.99953;2.24528;,
 -3.11470;-1.33126;1.25827;,
 -2.43272;-1.52555;-0.58775;,
 -2.63413;-1.93924;1.01104;,
 -0.40626;-0.73508;-1.88151;,
 1.58940;0.39863;-2.21091;,
 2.76397;0.66384;-1.42184;,
 2.58968;0.30585;-0.03834;,
 1.24481;-0.89299;1.83335;,
 -1.27678;-1.63277;1.92292;,
 -2.63413;-1.93924;1.01104;,
 -1.63408;-1.93545;-0.61544;,
 -1.78823;-2.25208;0.60823;,
 -0.29348;-1.32609;-1.63990;,
 1.51643;-0.30908;-1.89816;,
 2.41540;-0.10610;-1.29423;,
 2.28200;-0.38009;-0.23535;,
 1.35679;-1.69141;1.36638;,
 -0.82161;-2.28886;1.45916;,
 -1.78823;-2.25208;0.60823;,
 -0.67989;-2.32599;-0.61348;,
 -0.77299;-2.51723;0.12557;,
 0.35401;-1.90781;-1.25200;,
 1.24680;-1.14782;-1.50453;,
 1.80624;-1.02150;-1.12870;,
 1.69048;-1.37337;-0.19765;,
 1.18020;-2.16804;0.46596;,
 -0.14555;-2.37556;0.54708;,
 -0.77299;-2.51723;0.12557;,
 0.81610;-2.30739;-0.50637;,
 0.81610;-2.30739;-0.50637;,
 0.81610;-2.30739;-0.50637;,
 0.81610;-2.30739;-0.50637;,
 0.81610;-2.30739;-0.50637;,
 0.81610;-2.30739;-0.50637;,
 0.81610;-2.30739;-0.50637;,
 0.81610;-2.30739;-0.50637;;
 
 64;
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
 3;69,70,78;;
 
 MeshMaterialList {
  1;
  64;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\OneDrive\\�f�X�N�g�b�v\\���Ɛ���\\cos-mos\\data\\TEXTURE\\Stone.png";
   }
  }
 }
 MeshNormals {
  66;
  -0.351840;0.914310;0.200613;,
  -0.603269;0.733400;0.313354;,
  -0.646414;0.762985;-0.001657;,
  -0.501955;0.852211;-0.147574;,
  -0.193189;0.974371;-0.115236;,
  0.123927;0.989702;0.071637;,
  0.188360;0.886102;0.423489;,
  0.000963;0.717396;0.696665;,
  -0.327900;0.699895;0.634530;,
  -0.909040;0.344611;0.234284;,
  -0.904008;0.334227;-0.266573;,
  -0.566280;0.539124;-0.623436;,
  0.072043;0.849505;-0.522639;,
  0.541019;0.838808;-0.060823;,
  0.586646;0.685751;0.430803;,
  0.272480;0.400116;0.875021;,
  -0.237173;0.138011;0.961614;,
  -0.974952;-0.201089;-0.095037;,
  -0.912304;-0.076882;-0.402231;,
  -0.453573;0.137152;-0.880602;,
  0.154442;0.245934;-0.956903;,
  0.766559;0.616069;-0.181236;,
  0.792213;0.463446;0.397009;,
  0.369123;0.233011;0.899697;,
  -0.276283;-0.111318;0.954608;,
  -0.906090;-0.386341;-0.172455;,
  -0.790726;-0.409589;-0.454961;,
  -0.251003;-0.288133;-0.924109;,
  0.384796;-0.020739;-0.922769;,
  0.611596;0.096533;-0.785259;,
  0.934885;-0.000923;0.354949;,
  0.705241;-0.285592;0.648901;,
  -0.246510;-0.330537;0.911031;,
  -0.577180;-0.816591;-0.006531;,
  -0.524246;-0.675524;-0.518492;,
  -0.105915;-0.510865;-0.853112;,
  0.379116;-0.327125;-0.865598;,
  0.757181;-0.328293;-0.564713;,
  0.928942;-0.265740;0.257778;,
  0.572634;-0.430179;0.697880;,
  -0.096326;-0.643165;0.759645;,
  -0.308895;-0.950594;0.030901;,
  -0.365785;-0.754060;-0.545522;,
  -0.096479;-0.506916;-0.856579;,
  0.346915;-0.442891;-0.826739;,
  0.778687;-0.490930;-0.390684;,
  0.890846;-0.442948;0.100945;,
  0.612721;-0.627329;0.480658;,
  0.012722;-0.860258;0.509701;,
  -0.053654;-0.996094;-0.070132;,
  -0.131502;-0.881727;-0.453061;,
  0.095165;-0.696230;-0.711482;,
  0.413404;-0.586730;-0.696308;,
  0.697047;-0.611870;-0.373818;,
  0.807105;-0.584819;-0.081043;,
  0.546421;-0.832341;0.092912;,
  0.127160;-0.976111;0.176174;,
  0.384319;-0.878641;-0.283352;,
  0.510736;0.809006;-0.290961;,
  -0.403371;-0.497841;0.767754;,
  -0.484965;-0.298215;0.822117;,
  0.967533;0.093317;0.234888;,
  -0.132976;0.021504;0.990886;,
  -0.468397;-0.345265;0.813263;,
  0.973245;-0.151589;0.172671;,
  -0.359394;-0.581019;0.730242;;
  64;
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
  4;12,13,21,58;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,16,24,23;,
  4;16,59,60,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,61;,
  4;22,23,31,30;,
  4;23,24,32,62;,
  4;24,60,63,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;61,30,38,64;,
  4;30,31,39,38;,
  4;31,32,40,39;,
  4;32,63,65,40;,
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
  3;56,49,57;;
 }
 MeshTextureCoords {
  79;
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
  0.937500;1.000000;;
 }
}