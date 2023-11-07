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
 280;
 0.01594;2.09730;1.56745;,
 -0.50563;1.92895;2.03715;,
 0.01594;1.92895;2.23171;,
 0.01594;2.09730;1.56745;,
 -0.72166;1.92895;1.56745;,
 0.01594;2.09730;1.56745;,
 -0.50563;1.92895;1.09776;,
 0.01594;2.09730;1.56745;,
 0.01594;1.92895;0.90320;,
 0.01594;2.09730;1.56745;,
 0.53750;1.92895;1.09776;,
 0.01594;2.09730;1.56745;,
 0.75354;1.92895;1.56745;,
 0.01594;2.09730;1.56745;,
 0.53750;1.92895;2.03715;,
 0.01594;2.09730;1.56745;,
 0.01594;1.92895;2.23171;,
 -0.92389;1.45724;2.41382;,
 0.01594;1.45724;2.76440;,
 -1.31318;1.45724;1.56745;,
 -0.92389;1.45724;0.72108;,
 0.01594;1.45724;0.37051;,
 0.95576;1.45724;0.72108;,
 1.34505;1.45724;1.56745;,
 0.95576;1.45724;2.41382;,
 0.01594;1.45724;2.76440;,
 -1.15600;0.77559;2.62285;,
 0.01594;0.77559;3.06002;,
 -1.64144;0.77559;1.56745;,
 -1.15600;0.77559;0.51205;,
 0.01594;0.77559;0.07489;,
 1.18788;0.77559;0.51205;,
 1.67332;0.77559;1.56745;,
 1.18788;0.77559;2.62285;,
 0.01594;0.77559;3.06002;,
 -1.15600;0.01902;2.62285;,
 0.01594;0.01902;3.06002;,
 -1.64144;0.01902;1.56745;,
 -1.15600;0.01902;0.51205;,
 0.01594;0.01902;0.07489;,
 1.18788;0.01902;0.51205;,
 1.67332;0.01902;1.56745;,
 1.18788;0.01902;2.62285;,
 0.01594;0.01902;3.06002;,
 -0.92389;-0.66262;2.41382;,
 0.01594;-0.66262;2.76440;,
 -1.31318;-0.66262;1.56745;,
 -0.92389;-0.66262;0.72108;,
 0.01594;-0.66262;0.37051;,
 0.95576;-0.66262;0.72108;,
 1.34505;-0.66262;1.56745;,
 0.95576;-0.66262;2.41382;,
 0.01594;-0.66262;2.76440;,
 -0.50563;-1.13434;2.03715;,
 0.01594;-1.13434;2.23171;,
 -0.72166;-1.13434;1.56745;,
 -0.50563;-1.13434;1.09776;,
 0.01594;-1.13434;0.90320;,
 0.53750;-1.13434;1.09776;,
 0.75354;-1.13434;1.56745;,
 0.53750;-1.13434;2.03715;,
 0.01594;-1.13434;2.23171;,
 0.01594;-1.30269;1.56745;,
 0.01594;-1.30269;1.56745;,
 0.01594;-1.30269;1.56745;,
 0.01594;-1.30269;1.56745;,
 0.01594;-1.30269;1.56745;,
 0.01594;-1.30269;1.56745;,
 0.01594;-1.30269;1.56745;,
 0.01594;-1.30269;1.56745;,
 1.38949;2.84623;1.56745;,
 1.11266;2.75687;1.81676;,
 1.38949;2.75687;1.92002;,
 1.38949;2.84623;1.56745;,
 0.99799;2.75687;1.56745;,
 1.38949;2.84623;1.56745;,
 1.11266;2.75687;1.31815;,
 1.38949;2.84623;1.56745;,
 1.38949;2.75687;1.21488;,
 1.38949;2.84623;1.56745;,
 1.66632;2.75687;1.31815;,
 1.38949;2.84623;1.56745;,
 1.78099;2.75687;1.56745;,
 1.38949;2.84623;1.56745;,
 1.66632;2.75687;1.81676;,
 1.38949;2.84623;1.56745;,
 1.38949;2.75687;1.92002;,
 0.89066;2.50650;2.01668;,
 1.38949;2.50650;2.20276;,
 0.68403;2.50650;1.56745;,
 0.89066;2.50650;1.11822;,
 1.38949;2.50650;0.93215;,
 1.88833;2.50650;1.11822;,
 2.09495;2.50650;1.56745;,
 1.88833;2.50650;2.01668;,
 1.38949;2.50650;2.20276;,
 0.76746;2.14470;2.12763;,
 1.38949;2.14470;2.35967;,
 0.50980;2.14470;1.56745;,
 0.76746;2.14470;1.00728;,
 1.38949;2.14470;0.77524;,
 2.01153;2.14470;1.00728;,
 2.26918;2.14470;1.56745;,
 2.01153;2.14470;2.12763;,
 1.38949;2.14470;2.35967;,
 0.76746;1.74313;2.12763;,
 1.38949;1.74313;2.35967;,
 0.50980;1.74313;1.56745;,
 0.76746;1.74313;1.00728;,
 1.38949;1.74313;0.77524;,
 2.01153;1.74313;1.00728;,
 2.26918;1.74313;1.56745;,
 2.01153;1.74313;2.12763;,
 1.38949;1.74313;2.35967;,
 0.89066;1.38133;2.01668;,
 1.38949;1.38133;2.20276;,
 0.68403;1.38133;1.56745;,
 0.89066;1.38133;1.11822;,
 1.38949;1.38133;0.93215;,
 1.88833;1.38133;1.11822;,
 2.09495;1.38133;1.56745;,
 1.88833;1.38133;2.01668;,
 1.38949;1.38133;2.20276;,
 1.11266;1.13096;1.81676;,
 1.38949;1.13096;1.92002;,
 0.99799;1.13096;1.56745;,
 1.11266;1.13096;1.31815;,
 1.38949;1.13096;1.21488;,
 1.66632;1.13096;1.31815;,
 1.78099;1.13096;1.56745;,
 1.66632;1.13096;1.81676;,
 1.38949;1.13096;1.92002;,
 1.38949;1.04160;1.56745;,
 1.38949;1.04160;1.56745;,
 1.38949;1.04160;1.56745;,
 1.38949;1.04160;1.56745;,
 1.38949;1.04160;1.56745;,
 1.38949;1.04160;1.56745;,
 1.38949;1.04160;1.56745;,
 1.38949;1.04160;1.56745;,
 0.00472;3.62455;1.56745;,
 -0.27211;3.53519;1.81676;,
 0.00472;3.53519;1.92002;,
 0.00472;3.62455;1.56745;,
 -0.38678;3.53519;1.56745;,
 0.00472;3.62455;1.56745;,
 -0.27211;3.53519;1.31815;,
 0.00472;3.62455;1.56745;,
 0.00472;3.53519;1.21488;,
 0.00472;3.62455;1.56745;,
 0.28156;3.53519;1.31815;,
 0.00472;3.62455;1.56745;,
 0.39622;3.53519;1.56745;,
 0.00472;3.62455;1.56745;,
 0.28156;3.53519;1.81676;,
 0.00472;3.62455;1.56745;,
 0.00472;3.53519;1.92002;,
 -0.49411;3.28482;2.01668;,
 0.00472;3.28482;2.20276;,
 -0.70073;3.28482;1.56745;,
 -0.49411;3.28482;1.11822;,
 0.00472;3.28482;0.93215;,
 0.50356;3.28482;1.11822;,
 0.71018;3.28482;1.56745;,
 0.50356;3.28482;2.01668;,
 0.00472;3.28482;2.20276;,
 -0.61731;2.92302;2.12763;,
 0.00472;2.92302;2.35967;,
 -0.87497;2.92302;1.56745;,
 -0.61731;2.92302;1.00728;,
 0.00472;2.92302;0.77524;,
 0.62676;2.92302;1.00728;,
 0.88442;2.92302;1.56745;,
 0.62676;2.92302;2.12763;,
 0.00472;2.92302;2.35967;,
 -0.61731;2.52145;2.12763;,
 0.00472;2.52145;2.35967;,
 -0.87497;2.52145;1.56745;,
 -0.61731;2.52145;1.00728;,
 0.00472;2.52145;0.77524;,
 0.62676;2.52145;1.00728;,
 0.88442;2.52145;1.56745;,
 0.62676;2.52145;2.12763;,
 0.00472;2.52145;2.35967;,
 -0.49411;2.15965;2.01668;,
 0.00472;2.15965;2.20276;,
 -0.70073;2.15965;1.56745;,
 -0.49411;2.15965;1.11822;,
 0.00472;2.15965;0.93215;,
 0.50356;2.15965;1.11822;,
 0.71018;2.15965;1.56745;,
 0.50356;2.15965;2.01668;,
 0.00472;2.15965;2.20276;,
 -0.27211;1.90928;1.81676;,
 0.00472;1.90928;1.92002;,
 -0.38677;1.90928;1.56745;,
 -0.27211;1.90928;1.31815;,
 0.00472;1.90928;1.21488;,
 0.28156;1.90928;1.31815;,
 0.39622;1.90928;1.56745;,
 0.28156;1.90928;1.81676;,
 0.00472;1.90928;1.92002;,
 0.00472;1.81992;1.56745;,
 0.00472;1.81992;1.56745;,
 0.00472;1.81992;1.56745;,
 0.00472;1.81992;1.56745;,
 0.00472;1.81992;1.56745;,
 0.00472;1.81992;1.56745;,
 0.00472;1.81992;1.56745;,
 0.00472;1.81992;1.56745;,
 -1.43240;2.90208;1.56745;,
 -1.70924;2.81272;1.81676;,
 -1.43240;2.81272;1.92002;,
 -1.43240;2.90208;1.56745;,
 -1.82391;2.81272;1.56745;,
 -1.43240;2.90208;1.56745;,
 -1.70924;2.81272;1.31815;,
 -1.43240;2.90208;1.56745;,
 -1.43240;2.81272;1.21488;,
 -1.43240;2.90208;1.56745;,
 -1.15557;2.81272;1.31815;,
 -1.43240;2.90208;1.56745;,
 -1.04090;2.81272;1.56745;,
 -1.43240;2.90208;1.56745;,
 -1.15557;2.81272;1.81676;,
 -1.43240;2.90208;1.56745;,
 -1.43240;2.81272;1.92002;,
 -1.93124;2.56235;2.01668;,
 -1.43240;2.56235;2.20276;,
 -2.13786;2.56235;1.56745;,
 -1.93124;2.56235;1.11822;,
 -1.43240;2.56235;0.93215;,
 -0.93357;2.56235;1.11822;,
 -0.72695;2.56235;1.56745;,
 -0.93357;2.56235;2.01668;,
 -1.43240;2.56235;2.20276;,
 -2.05444;2.20055;2.12763;,
 -1.43240;2.20055;2.35967;,
 -2.31210;2.20055;1.56745;,
 -2.05444;2.20055;1.00728;,
 -1.43240;2.20055;0.77524;,
 -0.81037;2.20055;1.00728;,
 -0.55271;2.20055;1.56745;,
 -0.81037;2.20055;2.12763;,
 -1.43240;2.20055;2.35967;,
 -2.05444;1.79898;2.12763;,
 -1.43240;1.79898;2.35967;,
 -2.31210;1.79898;1.56745;,
 -2.05444;1.79898;1.00728;,
 -1.43240;1.79898;0.77524;,
 -0.81037;1.79898;1.00728;,
 -0.55271;1.79898;1.56745;,
 -0.81037;1.79898;2.12763;,
 -1.43240;1.79898;2.35967;,
 -1.93124;1.43718;2.01668;,
 -1.43240;1.43718;2.20276;,
 -2.13786;1.43718;1.56745;,
 -1.93124;1.43718;1.11822;,
 -1.43240;1.43718;0.93215;,
 -0.93357;1.43718;1.11822;,
 -0.72695;1.43718;1.56745;,
 -0.93357;1.43718;2.01668;,
 -1.43240;1.43718;2.20276;,
 -1.70924;1.18681;1.81676;,
 -1.43240;1.18681;1.92002;,
 -1.82391;1.18681;1.56745;,
 -1.70924;1.18681;1.31815;,
 -1.43240;1.18681;1.21488;,
 -1.15557;1.18681;1.31815;,
 -1.04090;1.18681;1.56745;,
 -1.15557;1.18681;1.81676;,
 -1.43240;1.18681;1.92002;,
 -1.43240;1.09745;1.56745;,
 -1.43240;1.09745;1.56745;,
 -1.43240;1.09745;1.56745;,
 -1.43240;1.09745;1.56745;,
 -1.43240;1.09745;1.56745;,
 -1.43240;1.09745;1.56745;,
 -1.43240;1.09745;1.56745;,
 -1.43240;1.09745;1.56745;;
 
 224;
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
 3;54,53,62;,
 3;53,55,63;,
 3;55,56,64;,
 3;56,57,65;,
 3;57,58,66;,
 3;58,59,67;,
 3;59,60,68;,
 3;60,61,69;,
 3;70,71,72;,
 3;73,74,71;,
 3;75,76,74;,
 3;77,78,76;,
 3;79,80,78;,
 3;81,82,80;,
 3;83,84,82;,
 3;85,86,84;,
 4;72,71,87,88;,
 4;71,74,89,87;,
 4;74,76,90,89;,
 4;76,78,91,90;,
 4;78,80,92,91;,
 4;80,82,93,92;,
 4;82,84,94,93;,
 4;84,86,95,94;,
 4;88,87,96,97;,
 4;87,89,98,96;,
 4;89,90,99,98;,
 4;90,91,100,99;,
 4;91,92,101,100;,
 4;92,93,102,101;,
 4;93,94,103,102;,
 4;94,95,104,103;,
 4;97,96,105,106;,
 4;96,98,107,105;,
 4;98,99,108,107;,
 4;99,100,109,108;,
 4;100,101,110,109;,
 4;101,102,111,110;,
 4;102,103,112,111;,
 4;103,104,113,112;,
 4;106,105,114,115;,
 4;105,107,116,114;,
 4;107,108,117,116;,
 4;108,109,118,117;,
 4;109,110,119,118;,
 4;110,111,120,119;,
 4;111,112,121,120;,
 4;112,113,122,121;,
 4;115,114,123,124;,
 4;114,116,125,123;,
 4;116,117,126,125;,
 4;117,118,127,126;,
 4;118,119,128,127;,
 4;119,120,129,128;,
 4;120,121,130,129;,
 4;121,122,131,130;,
 3;124,123,132;,
 3;123,125,133;,
 3;125,126,134;,
 3;126,127,135;,
 3;127,128,136;,
 3;128,129,137;,
 3;129,130,138;,
 3;130,131,139;,
 3;140,141,142;,
 3;143,144,141;,
 3;145,146,144;,
 3;147,148,146;,
 3;149,150,148;,
 3;151,152,150;,
 3;153,154,152;,
 3;155,156,154;,
 4;142,141,157,158;,
 4;141,144,159,157;,
 4;144,146,160,159;,
 4;146,148,161,160;,
 4;148,150,162,161;,
 4;150,152,163,162;,
 4;152,154,164,163;,
 4;154,156,165,164;,
 4;158,157,166,167;,
 4;157,159,168,166;,
 4;159,160,169,168;,
 4;160,161,170,169;,
 4;161,162,171,170;,
 4;162,163,172,171;,
 4;163,164,173,172;,
 4;164,165,174,173;,
 4;167,166,175,176;,
 4;166,168,177,175;,
 4;168,169,178,177;,
 4;169,170,179,178;,
 4;170,171,180,179;,
 4;171,172,181,180;,
 4;172,173,182,181;,
 4;173,174,183,182;,
 4;176,175,184,185;,
 4;175,177,186,184;,
 4;177,178,187,186;,
 4;178,179,188,187;,
 4;179,180,189,188;,
 4;180,181,190,189;,
 4;181,182,191,190;,
 4;182,183,192,191;,
 4;185,184,193,194;,
 4;184,186,195,193;,
 4;186,187,196,195;,
 4;187,188,197,196;,
 4;188,189,198,197;,
 4;189,190,199,198;,
 4;190,191,200,199;,
 4;191,192,201,200;,
 3;194,193,202;,
 3;193,195,203;,
 3;195,196,204;,
 3;196,197,205;,
 3;197,198,206;,
 3;198,199,207;,
 3;199,200,208;,
 3;200,201,209;,
 3;210,211,212;,
 3;213,214,211;,
 3;215,216,214;,
 3;217,218,216;,
 3;219,220,218;,
 3;221,222,220;,
 3;223,224,222;,
 3;225,226,224;,
 4;212,211,227,228;,
 4;211,214,229,227;,
 4;214,216,230,229;,
 4;216,218,231,230;,
 4;218,220,232,231;,
 4;220,222,233,232;,
 4;222,224,234,233;,
 4;224,226,235,234;,
 4;228,227,236,237;,
 4;227,229,238,236;,
 4;229,230,239,238;,
 4;230,231,240,239;,
 4;231,232,241,240;,
 4;232,233,242,241;,
 4;233,234,243,242;,
 4;234,235,244,243;,
 4;237,236,245,246;,
 4;236,238,247,245;,
 4;238,239,248,247;,
 4;239,240,249,248;,
 4;240,241,250,249;,
 4;241,242,251,250;,
 4;242,243,252,251;,
 4;243,244,253,252;,
 4;246,245,254,255;,
 4;245,247,256,254;,
 4;247,248,257,256;,
 4;248,249,258,257;,
 4;249,250,259,258;,
 4;250,251,260,259;,
 4;251,252,261,260;,
 4;252,253,262,261;,
 4;255,254,263,264;,
 4;254,256,265,263;,
 4;256,257,266,265;,
 4;257,258,267,266;,
 4;258,259,268,267;,
 4;259,260,269,268;,
 4;260,261,270,269;,
 4;261,262,271,270;,
 3;264,263,272;,
 3;263,265,273;,
 3;265,266,274;,
 3;266,267,275;,
 3;267,268,276;,
 3;268,269,277;,
 3;269,270,278;,
 3;270,271,279;;
 
 MeshMaterialList {
  6;
  224;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.561600;0.800000;0.254400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.737600;0.712000;0.153600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.119200;0.150400;0.800000;1.000000;;
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
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.668000;0.332800;0.712000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  200;
  0.000000;1.000000;-0.000001;,
  0.000001;0.885425;0.464783;,
  -0.301693;0.893932;0.331460;,
  -0.430348;0.902663;-0.000001;,
  -0.301692;0.893932;-0.331462;,
  0.000001;0.885425;-0.464783;,
  0.301693;0.893931;-0.331464;,
  0.430349;0.902662;-0.000001;,
  0.301695;0.893931;0.331463;,
  0.000001;0.589525;0.807750;,
  -0.537587;0.607644;0.584611;,
  -0.778997;0.627028;-0.000001;,
  -0.537584;0.607645;-0.584612;,
  0.000001;0.589526;-0.807749;,
  0.537583;0.607647;-0.584611;,
  0.778994;0.627031;-0.000001;,
  0.537586;0.607645;0.584610;,
  0.000000;0.204167;0.978936;,
  -0.663925;0.213759;0.716597;,
  -0.974503;0.224376;0.000000;,
  -0.663922;0.213759;-0.716599;,
  0.000000;0.204167;-0.978936;,
  0.663921;0.213763;-0.716599;,
  0.974501;0.224382;0.000000;,
  0.663924;0.213762;0.716596;,
  0.000000;-0.204169;0.978935;,
  -0.663924;-0.213761;0.716596;,
  -0.974502;-0.224378;0.000000;,
  -0.663922;-0.213762;-0.716599;,
  0.000000;-0.204169;-0.978936;,
  0.663921;-0.213765;-0.716599;,
  0.974501;-0.224384;0.000000;,
  0.663924;-0.213765;0.716596;,
  0.000001;-0.589523;0.807752;,
  -0.537588;-0.607641;0.584612;,
  -0.778999;-0.627025;-0.000000;,
  -0.537585;-0.607643;-0.584614;,
  0.000001;-0.589523;-0.807751;,
  0.537584;-0.607644;-0.584613;,
  0.778996;-0.627029;-0.000001;,
  0.537587;-0.607643;0.584611;,
  0.000001;-0.885422;0.464787;,
  -0.301697;-0.893930;0.331464;,
  -0.430353;-0.902661;-0.000001;,
  -0.301695;-0.893930;-0.331465;,
  0.000001;-0.885422;-0.464788;,
  0.301697;-0.893929;-0.331468;,
  0.430354;-0.902660;-0.000001;,
  0.301698;-0.893929;0.331466;,
  0.000000;-1.000000;-0.000001;,
  -0.000000;1.000000;-0.000001;,
  -0.000001;0.885421;0.464790;,
  -0.301696;0.893928;0.331469;,
  -0.430353;0.902661;-0.000002;,
  -0.301697;0.893928;-0.331470;,
  -0.000001;0.885421;-0.464790;,
  0.301694;0.893929;-0.331468;,
  0.430350;0.902662;-0.000002;,
  0.301693;0.893930;0.331466;,
  -0.000002;0.589527;0.807749;,
  -0.537586;0.607642;0.584613;,
  -0.778997;0.627028;-0.000002;,
  -0.537584;0.607640;-0.584617;,
  -0.000002;0.589521;-0.807753;,
  0.537583;0.607645;-0.584613;,
  0.778995;0.627030;-0.000002;,
  0.537585;0.607647;0.584609;,
  -0.000003;0.204170;0.978935;,
  -0.663926;0.213761;0.716595;,
  -0.974503;0.224376;-0.000003;,
  -0.663922;0.213757;-0.716600;,
  -0.000003;0.204165;-0.978936;,
  0.663925;0.213759;-0.716597;,
  0.974503;0.224376;-0.000003;,
  0.663928;0.213762;0.716592;,
  -0.000003;-0.204171;0.978935;,
  -0.663926;-0.213762;0.716595;,
  -0.974503;-0.224377;-0.000003;,
  -0.663922;-0.213758;-0.716599;,
  -0.000003;-0.204166;-0.978936;,
  0.663925;-0.213759;-0.716597;,
  0.974503;-0.224376;-0.000003;,
  0.663928;-0.213763;0.716592;,
  -0.000002;-0.589528;0.807748;,
  -0.537585;-0.607645;0.584612;,
  -0.778996;-0.627029;-0.000002;,
  -0.537584;-0.607641;-0.584616;,
  -0.000002;-0.589522;-0.807752;,
  0.537583;-0.607645;-0.584613;,
  0.778995;-0.627031;-0.000002;,
  0.537584;-0.607648;0.584608;,
  -0.000001;-0.885422;0.464788;,
  -0.301694;-0.893930;0.331467;,
  -0.430352;-0.902661;-0.000002;,
  -0.301697;-0.893928;-0.331470;,
  -0.000001;-0.885421;-0.464790;,
  0.301694;-0.893929;-0.331468;,
  0.430349;-0.902663;-0.000002;,
  0.301692;-0.893931;0.331466;,
  -0.000000;-1.000000;-0.000001;,
  -0.000001;1.000000;-0.000001;,
  -0.000003;0.885420;0.464791;,
  -0.301697;0.893928;0.331468;,
  -0.430357;0.902659;-0.000002;,
  -0.301699;0.893928;-0.331468;,
  -0.000003;0.885422;-0.464788;,
  0.301696;0.893931;-0.331463;,
  0.430354;0.902660;-0.000002;,
  0.301694;0.893931;0.331463;,
  -0.000003;0.589526;0.807750;,
  -0.537587;0.607645;0.584609;,
  -0.778997;0.627028;-0.000002;,
  -0.537586;0.607642;-0.584613;,
  -0.000003;0.589523;-0.807752;,
  0.537583;0.607646;-0.584612;,
  0.778994;0.627031;-0.000002;,
  0.537584;0.607648;0.584608;,
  -0.000003;0.204171;0.978935;,
  -0.663926;0.213764;0.716594;,
  -0.974501;0.224382;-0.000003;,
  -0.663923;0.213759;-0.716598;,
  -0.000003;0.204164;-0.978937;,
  0.663921;0.213759;-0.716600;,
  0.974501;0.224382;-0.000003;,
  0.663924;0.213765;0.716596;,
  -0.000003;-0.204171;0.978935;,
  -0.663926;-0.213765;0.716594;,
  -0.974501;-0.224382;-0.000003;,
  -0.663923;-0.213761;-0.716598;,
  -0.000003;-0.204165;-0.978937;,
  0.663921;-0.213759;-0.716600;,
  0.974502;-0.224381;-0.000003;,
  0.663924;-0.213764;0.716596;,
  -0.000003;-0.589525;0.807750;,
  -0.537588;-0.607646;0.584607;,
  -0.778995;-0.627030;-0.000002;,
  -0.537586;-0.607646;-0.584609;,
  -0.000003;-0.589522;-0.807753;,
  0.537584;-0.607644;-0.584613;,
  0.778995;-0.627030;-0.000002;,
  0.537585;-0.607647;0.584609;,
  -0.000003;-0.885420;0.464792;,
  -0.301699;-0.893929;0.331464;,
  -0.430358;-0.902658;-0.000002;,
  -0.301700;-0.893929;-0.331464;,
  -0.000002;-0.885421;-0.464790;,
  0.301697;-0.893930;-0.331465;,
  0.430355;-0.902660;-0.000002;,
  0.301695;-0.893931;0.331464;,
  -0.000002;-1.000000;-0.000001;,
  0.000002;1.000000;-0.000001;,
  0.000003;0.885421;0.464791;,
  -0.301694;0.893930;0.331468;,
  -0.430354;0.902660;-0.000002;,
  -0.301695;0.893929;-0.331469;,
  0.000003;0.885421;-0.464790;,
  0.301699;0.893927;-0.331470;,
  0.430357;0.902659;-0.000002;,
  0.301698;0.893928;0.331469;,
  0.000003;0.589526;0.807749;,
  -0.537585;0.607646;0.584610;,
  -0.778996;0.627028;-0.000002;,
  -0.537583;0.607643;-0.584614;,
  0.000003;0.589521;-0.807753;,
  0.537586;0.607641;-0.584614;,
  0.778997;0.627028;-0.000002;,
  0.537587;0.607644;0.584609;,
  0.000002;0.204171;0.978935;,
  -0.663925;0.213765;0.716595;,
  -0.974501;0.224382;-0.000003;,
  -0.663921;0.213760;-0.716600;,
  0.000002;0.204164;-0.978937;,
  0.663923;0.213759;-0.716598;,
  0.974501;0.224382;-0.000003;,
  0.663926;0.213765;0.716594;,
  0.000002;-0.204171;0.978935;,
  -0.663925;-0.213764;0.716595;,
  -0.974501;-0.224381;-0.000003;,
  -0.663921;-0.213760;-0.716600;,
  0.000002;-0.204165;-0.978937;,
  0.663923;-0.213760;-0.716598;,
  0.974501;-0.224382;-0.000003;,
  0.663926;-0.213765;0.716594;,
  0.000003;-0.589528;0.807748;,
  -0.537585;-0.607646;0.584610;,
  -0.778997;-0.627027;-0.000002;,
  -0.537584;-0.607643;-0.584614;,
  0.000003;-0.589522;-0.807753;,
  0.537586;-0.607642;-0.584613;,
  0.778996;-0.627028;-0.000002;,
  0.537587;-0.607646;0.584608;,
  0.000002;-0.885421;0.464789;,
  -0.301694;-0.893929;0.331468;,
  -0.430355;-0.902660;-0.000002;,
  -0.301695;-0.893929;-0.331469;,
  0.000003;-0.885421;-0.464790;,
  0.301699;-0.893927;-0.331470;,
  0.430357;-0.902659;-0.000002;,
  0.301697;-0.893929;0.331467;,
  0.000002;-1.000000;-0.000001;;
  224;
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
  3;41,42,49;,
  3;42,43,49;,
  3;43,44,49;,
  3;44,45,49;,
  3;45,46,49;,
  3;46,47,49;,
  3;47,48,49;,
  3;48,41,49;,
  3;50,52,51;,
  3;50,53,52;,
  3;50,54,53;,
  3;50,55,54;,
  3;50,56,55;,
  3;50,57,56;,
  3;50,58,57;,
  3;50,51,58;,
  4;51,52,60,59;,
  4;52,53,61,60;,
  4;53,54,62,61;,
  4;54,55,63,62;,
  4;55,56,64,63;,
  4;56,57,65,64;,
  4;57,58,66,65;,
  4;58,51,59,66;,
  4;59,60,68,67;,
  4;60,61,69,68;,
  4;61,62,70,69;,
  4;62,63,71,70;,
  4;63,64,72,71;,
  4;64,65,73,72;,
  4;65,66,74,73;,
  4;66,59,67,74;,
  4;67,68,76,75;,
  4;68,69,77,76;,
  4;69,70,78,77;,
  4;70,71,79,78;,
  4;71,72,80,79;,
  4;72,73,81,80;,
  4;73,74,82,81;,
  4;74,67,75,82;,
  4;75,76,84,83;,
  4;76,77,85,84;,
  4;77,78,86,85;,
  4;78,79,87,86;,
  4;79,80,88,87;,
  4;80,81,89,88;,
  4;81,82,90,89;,
  4;82,75,83,90;,
  4;83,84,92,91;,
  4;84,85,93,92;,
  4;85,86,94,93;,
  4;86,87,95,94;,
  4;87,88,96,95;,
  4;88,89,97,96;,
  4;89,90,98,97;,
  4;90,83,91,98;,
  3;91,92,99;,
  3;92,93,99;,
  3;93,94,99;,
  3;94,95,99;,
  3;95,96,99;,
  3;96,97,99;,
  3;97,98,99;,
  3;98,91,99;,
  3;100,102,101;,
  3;100,103,102;,
  3;100,104,103;,
  3;100,105,104;,
  3;100,106,105;,
  3;100,107,106;,
  3;100,108,107;,
  3;100,101,108;,
  4;101,102,110,109;,
  4;102,103,111,110;,
  4;103,104,112,111;,
  4;104,105,113,112;,
  4;105,106,114,113;,
  4;106,107,115,114;,
  4;107,108,116,115;,
  4;108,101,109,116;,
  4;109,110,118,117;,
  4;110,111,119,118;,
  4;111,112,120,119;,
  4;112,113,121,120;,
  4;113,114,122,121;,
  4;114,115,123,122;,
  4;115,116,124,123;,
  4;116,109,117,124;,
  4;117,118,126,125;,
  4;118,119,127,126;,
  4;119,120,128,127;,
  4;120,121,129,128;,
  4;121,122,130,129;,
  4;122,123,131,130;,
  4;123,124,132,131;,
  4;124,117,125,132;,
  4;125,126,134,133;,
  4;126,127,135,134;,
  4;127,128,136,135;,
  4;128,129,137,136;,
  4;129,130,138,137;,
  4;130,131,139,138;,
  4;131,132,140,139;,
  4;132,125,133,140;,
  4;133,134,142,141;,
  4;134,135,143,142;,
  4;135,136,144,143;,
  4;136,137,145,144;,
  4;137,138,146,145;,
  4;138,139,147,146;,
  4;139,140,148,147;,
  4;140,133,141,148;,
  3;141,142,149;,
  3;142,143,149;,
  3;143,144,149;,
  3;144,145,149;,
  3;145,146,149;,
  3;146,147,149;,
  3;147,148,149;,
  3;148,141,149;,
  3;150,152,151;,
  3;150,153,152;,
  3;150,154,153;,
  3;150,155,154;,
  3;150,156,155;,
  3;150,157,156;,
  3;150,158,157;,
  3;150,151,158;,
  4;151,152,160,159;,
  4;152,153,161,160;,
  4;153,154,162,161;,
  4;154,155,163,162;,
  4;155,156,164,163;,
  4;156,157,165,164;,
  4;157,158,166,165;,
  4;158,151,159,166;,
  4;159,160,168,167;,
  4;160,161,169,168;,
  4;161,162,170,169;,
  4;162,163,171,170;,
  4;163,164,172,171;,
  4;164,165,173,172;,
  4;165,166,174,173;,
  4;166,159,167,174;,
  4;167,168,176,175;,
  4;168,169,177,176;,
  4;169,170,178,177;,
  4;170,171,179,178;,
  4;171,172,180,179;,
  4;172,173,181,180;,
  4;173,174,182,181;,
  4;174,167,175,182;,
  4;175,176,184,183;,
  4;176,177,185,184;,
  4;177,178,186,185;,
  4;178,179,187,186;,
  4;179,180,188,187;,
  4;180,181,189,188;,
  4;181,182,190,189;,
  4;182,175,183,190;,
  4;183,184,192,191;,
  4;184,185,193,192;,
  4;185,186,194,193;,
  4;186,187,195,194;,
  4;187,188,196,195;,
  4;188,189,197,196;,
  4;189,190,198,197;,
  4;190,183,191,198;,
  3;191,192,199;,
  3;192,193,199;,
  3;193,194,199;,
  3;194,195,199;,
  3;195,196,199;,
  3;196,197,199;,
  3;197,198,199;,
  3;198,191,199;;
 }
 MeshTextureCoords {
  280;
  0.062500;0.000000;,
  0.125000;0.142860;,
  0.000000;0.142860;,
  0.187500;0.000000;,
  0.250000;0.142860;,
  0.312500;0.000000;,
  0.375000;0.142860;,
  0.437500;0.000000;,
  0.500000;0.142860;,
  0.562500;0.000000;,
  0.625000;0.142860;,
  0.687500;0.000000;,
  0.750000;0.142860;,
  0.812500;0.000000;,
  0.875000;0.142860;,
  0.937500;0.000000;,
  1.000000;0.142860;,
  0.125000;0.285710;,
  0.000000;0.285710;,
  0.250000;0.285710;,
  0.375000;0.285710;,
  0.500000;0.285710;,
  0.625000;0.285710;,
  0.750000;0.285710;,
  0.875000;0.285710;,
  1.000000;0.285710;,
  0.125000;0.428570;,
  0.000000;0.428570;,
  0.250000;0.428570;,
  0.375000;0.428570;,
  0.500000;0.428570;,
  0.625000;0.428570;,
  0.750000;0.428570;,
  0.875000;0.428570;,
  1.000000;0.428570;,
  0.125000;0.571430;,
  0.000000;0.571430;,
  0.250000;0.571430;,
  0.375000;0.571430;,
  0.500000;0.571430;,
  0.625000;0.571430;,
  0.750000;0.571430;,
  0.875000;0.571430;,
  1.000000;0.571430;,
  0.125000;0.714290;,
  0.000000;0.714290;,
  0.250000;0.714290;,
  0.375000;0.714290;,
  0.500000;0.714290;,
  0.625000;0.714290;,
  0.750000;0.714290;,
  0.875000;0.714290;,
  1.000000;0.714290;,
  0.125000;0.857140;,
  0.000000;0.857140;,
  0.250000;0.857140;,
  0.375000;0.857140;,
  0.500000;0.857140;,
  0.625000;0.857140;,
  0.750000;0.857140;,
  0.875000;0.857140;,
  1.000000;0.857140;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.062500;0.000000;,
  0.125000;0.142860;,
  0.000000;0.142860;,
  0.187500;0.000000;,
  0.250000;0.142860;,
  0.312500;0.000000;,
  0.375000;0.142860;,
  0.437500;0.000000;,
  0.500000;0.142860;,
  0.562500;0.000000;,
  0.625000;0.142860;,
  0.687500;0.000000;,
  0.750000;0.142860;,
  0.812500;0.000000;,
  0.875000;0.142860;,
  0.937500;0.000000;,
  1.000000;0.142860;,
  0.125000;0.285710;,
  0.000000;0.285710;,
  0.250000;0.285710;,
  0.375000;0.285710;,
  0.500000;0.285710;,
  0.625000;0.285710;,
  0.750000;0.285710;,
  0.875000;0.285710;,
  1.000000;0.285710;,
  0.125000;0.428570;,
  0.000000;0.428570;,
  0.250000;0.428570;,
  0.375000;0.428570;,
  0.500000;0.428570;,
  0.625000;0.428570;,
  0.750000;0.428570;,
  0.875000;0.428570;,
  1.000000;0.428570;,
  0.125000;0.571430;,
  0.000000;0.571430;,
  0.250000;0.571430;,
  0.375000;0.571430;,
  0.500000;0.571430;,
  0.625000;0.571430;,
  0.750000;0.571430;,
  0.875000;0.571430;,
  1.000000;0.571430;,
  0.125000;0.714290;,
  0.000000;0.714290;,
  0.250000;0.714290;,
  0.375000;0.714290;,
  0.500000;0.714290;,
  0.625000;0.714290;,
  0.750000;0.714290;,
  0.875000;0.714290;,
  1.000000;0.714290;,
  0.125000;0.857140;,
  0.000000;0.857140;,
  0.250000;0.857140;,
  0.375000;0.857140;,
  0.500000;0.857140;,
  0.625000;0.857140;,
  0.750000;0.857140;,
  0.875000;0.857140;,
  1.000000;0.857140;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.062500;0.000000;,
  0.125000;0.142860;,
  0.000000;0.142860;,
  0.187500;0.000000;,
  0.250000;0.142860;,
  0.312500;0.000000;,
  0.375000;0.142860;,
  0.437500;0.000000;,
  0.500000;0.142860;,
  0.562500;0.000000;,
  0.625000;0.142860;,
  0.687500;0.000000;,
  0.750000;0.142860;,
  0.812500;0.000000;,
  0.875000;0.142860;,
  0.937500;0.000000;,
  1.000000;0.142860;,
  0.125000;0.285710;,
  0.000000;0.285710;,
  0.250000;0.285710;,
  0.375000;0.285710;,
  0.500000;0.285710;,
  0.625000;0.285710;,
  0.750000;0.285710;,
  0.875000;0.285710;,
  1.000000;0.285710;,
  0.125000;0.428570;,
  0.000000;0.428570;,
  0.250000;0.428570;,
  0.375000;0.428570;,
  0.500000;0.428570;,
  0.625000;0.428570;,
  0.750000;0.428570;,
  0.875000;0.428570;,
  1.000000;0.428570;,
  0.125000;0.571430;,
  0.000000;0.571430;,
  0.250000;0.571430;,
  0.375000;0.571430;,
  0.500000;0.571430;,
  0.625000;0.571430;,
  0.750000;0.571430;,
  0.875000;0.571430;,
  1.000000;0.571430;,
  0.125000;0.714290;,
  0.000000;0.714290;,
  0.250000;0.714290;,
  0.375000;0.714290;,
  0.500000;0.714290;,
  0.625000;0.714290;,
  0.750000;0.714290;,
  0.875000;0.714290;,
  1.000000;0.714290;,
  0.125000;0.857140;,
  0.000000;0.857140;,
  0.250000;0.857140;,
  0.375000;0.857140;,
  0.500000;0.857140;,
  0.625000;0.857140;,
  0.750000;0.857140;,
  0.875000;0.857140;,
  1.000000;0.857140;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.062500;0.000000;,
  0.125000;0.142860;,
  0.000000;0.142860;,
  0.187500;0.000000;,
  0.250000;0.142860;,
  0.312500;0.000000;,
  0.375000;0.142860;,
  0.437500;0.000000;,
  0.500000;0.142860;,
  0.562500;0.000000;,
  0.625000;0.142860;,
  0.687500;0.000000;,
  0.750000;0.142860;,
  0.812500;0.000000;,
  0.875000;0.142860;,
  0.937500;0.000000;,
  1.000000;0.142860;,
  0.125000;0.285710;,
  0.000000;0.285710;,
  0.250000;0.285710;,
  0.375000;0.285710;,
  0.500000;0.285710;,
  0.625000;0.285710;,
  0.750000;0.285710;,
  0.875000;0.285710;,
  1.000000;0.285710;,
  0.125000;0.428570;,
  0.000000;0.428570;,
  0.250000;0.428570;,
  0.375000;0.428570;,
  0.500000;0.428570;,
  0.625000;0.428570;,
  0.750000;0.428570;,
  0.875000;0.428570;,
  1.000000;0.428570;,
  0.125000;0.571430;,
  0.000000;0.571430;,
  0.250000;0.571430;,
  0.375000;0.571430;,
  0.500000;0.571430;,
  0.625000;0.571430;,
  0.750000;0.571430;,
  0.875000;0.571430;,
  1.000000;0.571430;,
  0.125000;0.714290;,
  0.000000;0.714290;,
  0.250000;0.714290;,
  0.375000;0.714290;,
  0.500000;0.714290;,
  0.625000;0.714290;,
  0.750000;0.714290;,
  0.875000;0.714290;,
  1.000000;0.714290;,
  0.125000;0.857140;,
  0.000000;0.857140;,
  0.250000;0.857140;,
  0.375000;0.857140;,
  0.500000;0.857140;,
  0.625000;0.857140;,
  0.750000;0.857140;,
  0.875000;0.857140;,
  1.000000;0.857140;,
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
