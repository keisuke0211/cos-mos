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
 444;
 0.00000;7.26727;0.00000;,
 6.47965;0.00000;0.00000;,
 6.07468;0.00000;1.25000;,
 0.00000;6.81307;1.25000;,
 -0.00000;-7.26727;-0.00000;,
 -0.00000;-6.81307;1.25000;,
 -6.47965;0.00000;0.00000;,
 -6.07468;0.00000;1.25000;,
 0.00000;7.26727;0.00000;,
 0.00000;6.81307;1.25000;,
 5.64812;-0.49415;1.50000;,
 0.55421;6.33467;1.50000;,
 -0.55421;-6.33467;1.50000;,
 -5.64812;0.49415;1.50000;,
 0.55421;6.33467;1.50000;,
 5.24469;-0.45885;2.75000;,
 0.51463;5.88219;2.75000;,
 -0.51463;-5.88219;2.75000;,
 -5.24469;0.45885;2.75000;,
 0.51463;5.88219;2.75000;,
 4.78591;-0.84389;3.00000;,
 0.94646;5.36765;3.00000;,
 -0.94646;-5.36765;3.00000;,
 -4.78591;0.84389;3.00000;,
 0.94646;5.36765;3.00000;,
 4.38709;-0.77356;4.25000;,
 0.86759;4.92035;4.25000;,
 -0.86759;-4.92035;4.25000;,
 -4.38709;0.77356;4.25000;,
 0.86759;4.92035;4.25000;,
 3.91179;-1.04816;4.50000;,
 1.17557;4.38728;4.50000;,
 -1.17557;-4.38728;4.50000;,
 -3.91179;1.04816;4.50000;,
 1.17557;4.38728;4.50000;,
 3.52061;-0.94335;5.75000;,
 1.05801;3.94855;5.75000;,
 -1.05801;-3.94855;5.75000;,
 -3.52061;0.94335;5.75000;,
 1.05801;3.94855;5.75000;,
 3.04444;-1.10809;6.00000;,
 1.24278;3.41450;6.00000;,
 -1.24278;-3.41450;6.00000;,
 -3.04444;1.10809;6.00000;,
 1.24278;3.41450;6.00000;,
 2.66389;-0.96958;7.25000;,
 1.08743;2.98769;7.25000;,
 -1.08743;-2.98769;7.25000;,
 -2.66389;0.96958;7.25000;,
 1.08743;2.98769;7.25000;,
 2.20221;-1.02691;7.50000;,
 1.15173;2.46990;7.50000;,
 -1.15173;-2.46990;7.50000;,
 -2.20221;1.02691;7.50000;,
 1.15173;2.46990;7.50000;,
 1.83518;-0.85576;8.75000;,
 0.95978;2.05825;8.75000;,
 -0.95978;-2.05825;8.75000;,
 -1.83518;0.85576;8.75000;,
 0.95978;2.05825;8.75000;,
 1.40289;-0.80996;9.00000;,
 0.90841;1.57341;9.00000;,
 -0.90841;-1.57341;9.00000;,
 -1.40289;0.80996;9.00000;,
 0.90841;1.57341;9.00000;,
 1.05217;-0.60747;10.25000;,
 0.68131;1.18006;10.25000;,
 -0.68131;-1.18006;10.25000;,
 -1.05217;0.60747;10.25000;,
 0.68131;1.18006;10.25000;,
 0.66348;-0.46457;10.50000;,
 0.52104;0.74413;10.50000;,
 -0.52104;-0.74413;10.50000;,
 -0.66348;0.46457;10.50000;,
 0.52104;0.74413;10.50000;,
 0.33174;-0.23229;11.75000;,
 0.26052;0.37206;11.75000;,
 -0.26052;-0.37206;11.75000;,
 -0.33174;0.23229;11.75000;,
 0.26052;0.37206;11.75000;,
 -0.00000;-0.00000;12.00000;,
 -0.00000;-0.00000;12.00000;,
 -0.00000;-0.00000;12.00000;,
 -0.00000;-0.00000;12.00000;,
 4.16896;2.73600;-0.36052;,
 7.57025;-0.44049;-0.09725;,
 7.13195;-0.92216;-0.20348;,
 3.73066;2.25433;-0.46676;,
 7.57025;-0.44049;-0.09725;,
 7.47966;-0.50157;0.55349;,
 7.04136;-0.98325;0.44726;,
 7.13195;-0.92216;-0.20348;,
 7.47966;-0.50157;0.55349;,
 4.07837;2.67492;0.29021;,
 3.64007;2.19324;0.18398;,
 7.04136;-0.98325;0.44726;,
 4.07837;2.67492;0.29021;,
 4.16896;2.73600;-0.36052;,
 3.73066;2.25433;-0.46676;,
 3.64007;2.19324;0.18398;,
 7.57025;-0.44049;-0.09725;,
 4.16896;2.73600;-0.36052;,
 3.73066;2.25433;-0.46676;,
 7.13195;-0.92216;-0.20348;,
 4.07835;3.34989;-0.03674;,
 7.02530;-0.11278;-0.20579;,
 6.53926;-0.54962;-0.11451;,
 3.59231;2.91305;0.05453;,
 7.02530;-0.11278;-0.20579;,
 7.09246;-0.05095;0.44772;,
 6.60642;-0.48780;0.53899;,
 6.53926;-0.54962;-0.11451;,
 7.09246;-0.05095;0.44772;,
 4.14550;3.41172;0.61676;,
 3.65947;2.97487;0.70803;,
 6.60642;-0.48780;0.53899;,
 4.14550;3.41172;0.61676;,
 4.07835;3.34989;-0.03674;,
 3.59231;2.91305;0.05453;,
 3.65947;2.97487;0.70803;,
 7.02530;-0.11278;-0.20579;,
 4.07835;3.34989;-0.03674;,
 3.59231;2.91305;0.05453;,
 6.53926;-0.54962;-0.11451;,
 -0.88057;7.19816;-0.22125;,
 3.63741;3.41739;-0.07174;,
 3.23225;2.92637;-0.24537;,
 -1.28573;6.70714;-0.39487;,
 3.63741;3.41739;-0.07174;,
 3.51356;3.29456;0.56464;,
 3.10841;2.80353;0.39101;,
 3.23225;2.92637;-0.24537;,
 3.51356;3.29456;0.56464;,
 -1.00442;7.07533;0.41513;,
 -1.40958;6.58430;0.24150;,
 3.10841;2.80353;0.39101;,
 -1.00442;7.07533;0.41513;,
 -0.88057;7.19816;-0.22125;,
 -1.28573;6.70714;-0.39487;,
 -1.40958;6.58430;0.24150;,
 3.63741;3.41739;-0.07174;,
 -0.88057;7.19816;-0.22125;,
 -1.28573;6.70714;-0.39487;,
 3.23225;2.92637;-0.24537;,
 4.09737;-0.61652;-0.04666;,
 5.22098;-2.37168;-0.37996;,
 4.72723;-2.75148;-0.16235;,
 3.72019;-0.90664;0.11958;,
 5.22098;-2.37168;-0.37996;,
 5.38158;-2.22370;0.24271;,
 4.88783;-2.60350;0.46032;,
 4.72723;-2.75148;-0.16235;,
 5.38158;-2.22370;0.24271;,
 4.22005;-0.50348;0.42899;,
 3.84287;-0.79360;0.59522;,
 4.88783;-2.60350;0.46032;,
 4.22005;-0.50348;0.42899;,
 4.09737;-0.61652;-0.04666;,
 3.72019;-0.90664;0.11958;,
 3.84287;-0.79360;0.59522;,
 5.22098;-2.37168;-0.37996;,
 4.09737;-0.61652;-0.04666;,
 3.72019;-0.90664;0.11958;,
 4.72723;-2.75148;-0.16235;,
 4.00196;-2.31721;-0.29864;,
 5.71586;0.77442;-0.15759;,
 6.22687;0.47662;-0.45014;,
 4.51298;-2.61501;-0.59119;,
 5.71586;0.77442;-0.15759;,
 5.97441;0.63714;0.43377;,
 6.48542;0.33935;0.14123;,
 6.22687;0.47662;-0.45014;,
 5.97441;0.63714;0.43377;,
 4.26051;-2.45448;0.29272;,
 4.77152;-2.75228;0.00017;,
 6.48542;0.33935;0.14123;,
 4.26051;-2.45448;0.29272;,
 4.00196;-2.31721;-0.29864;,
 4.51298;-2.61501;-0.59119;,
 4.77152;-2.75228;0.00017;,
 5.71586;0.77442;-0.15759;,
 4.00196;-2.31721;-0.29864;,
 4.51298;-2.61501;-0.59119;,
 6.22687;0.47662;-0.45014;,
 3.87246;-1.34293;-0.52680;,
 6.37563;1.12838;-0.14962;,
 6.78538;0.64891;0.04427;,
 4.28222;-1.82241;-0.33291;,
 6.37563;1.12838;-0.14962;,
 6.21024;1.24128;0.47911;,
 6.61999;0.76180;0.67300;,
 6.78538;0.64891;0.04427;,
 6.21024;1.24128;0.47911;,
 3.70706;-1.23003;0.10192;,
 4.11683;-1.70952;0.29582;,
 6.61999;0.76180;0.67300;,
 3.70706;-1.23003;0.10192;,
 3.87246;-1.34293;-0.52680;,
 4.28222;-1.82241;-0.33291;,
 4.11683;-1.70952;0.29582;,
 6.37563;1.12838;-0.14962;,
 3.87246;-1.34293;-0.52680;,
 4.28222;-1.82241;-0.33291;,
 6.78538;0.64891;0.04427;,
 -3.48048;-2.71858;0.10766;,
 -0.05757;-7.38566;-1.00169;,
 -0.50119;-7.77964;-0.71295;,
 -3.92410;-3.11258;0.39640;,
 -0.05757;-7.38566;-1.00169;,
 0.24527;-7.30146;-0.42152;,
 -0.19835;-7.69545;-0.13278;,
 -0.50119;-7.77964;-0.71295;,
 0.24527;-7.30146;-0.42152;,
 -3.17765;-2.63438;0.68783;,
 -3.62126;-3.02837;0.97658;,
 -0.19835;-7.69545;-0.13278;,
 -3.17765;-2.63438;0.68783;,
 -3.48048;-2.71858;0.10766;,
 -3.92410;-3.11258;0.39640;,
 -3.62126;-3.02837;0.97658;,
 -0.05757;-7.38566;-1.00169;,
 -3.48048;-2.71858;0.10766;,
 -3.92410;-3.11258;0.39640;,
 -0.50119;-7.77964;-0.71295;,
 3.43404;-3.36015;-0.31868;,
 0.09534;-8.21352;-0.15569;,
 -0.42957;-7.85861;-0.33982;,
 2.90913;-3.00523;-0.50281;,
 0.09534;-8.21352;-0.15569;,
 -0.04648;-8.09468;0.47769;,
 -0.57140;-7.73977;0.29356;,
 -0.42957;-7.85861;-0.33982;,
 -0.04648;-8.09468;0.47769;,
 3.29222;-3.24131;0.31470;,
 2.76731;-2.88640;0.13057;,
 -0.57140;-7.73977;0.29356;,
 3.29222;-3.24131;0.31470;,
 3.43404;-3.36015;-0.31868;,
 2.90913;-3.00523;-0.50281;,
 2.76731;-2.88640;0.13057;,
 0.09534;-8.21352;-0.15569;,
 3.43404;-3.36015;-0.31868;,
 2.90913;-3.00523;-0.50281;,
 -0.42957;-7.85861;-0.33982;,
 4.60248;-2.30061;-0.56865;,
 1.48511;-7.13468;0.71297;,
 1.05973;-6.98835;0.23023;,
 4.17710;-2.15428;-1.05140;,
 1.48511;-7.13468;0.71297;,
 1.12094;-6.78680;1.13931;,
 0.69556;-6.64046;0.65657;,
 1.05973;-6.98835;0.23023;,
 1.12094;-6.78680;1.13931;,
 4.23832;-1.95274;-0.14231;,
 3.81294;-1.80641;-0.62506;,
 0.69556;-6.64046;0.65657;,
 4.23832;-1.95274;-0.14231;,
 4.60248;-2.30061;-0.56865;,
 4.17710;-2.15428;-1.05140;,
 3.81294;-1.80641;-0.62506;,
 1.48511;-7.13468;0.71297;,
 4.60248;-2.30061;-0.56865;,
 4.17710;-2.15428;-1.05140;,
 1.05973;-6.98835;0.23023;,
 0.00488;7.38981;-0.19133;,
 5.22525;2.60649;0.35476;,
 4.95312;2.26164;-0.13761;,
 -0.26724;7.04494;-0.68370;,
 5.22525;2.60649;0.35476;,
 4.85671;2.27523;0.79047;,
 4.58459;1.93037;0.29811;,
 4.95312;2.26164;-0.13761;,
 4.85671;2.27523;0.79047;,
 -0.36365;7.05853;0.24438;,
 -0.63578;6.71368;-0.24799;,
 4.58459;1.93037;0.29811;,
 -0.36365;7.05853;0.24438;,
 0.00488;7.38981;-0.19133;,
 -0.26724;7.04494;-0.68370;,
 -0.63578;6.71368;-0.24799;,
 5.22525;2.60649;0.35476;,
 0.00488;7.38981;-0.19133;,
 -0.26724;7.04494;-0.68370;,
 4.95312;2.26164;-0.13761;,
 -6.23397;-0.01347;-0.51723;,
 -3.06569;-3.48510;-0.23688;,
 -3.45485;-3.83520;-0.17426;,
 -6.62314;-0.36356;-0.45461;,
 -3.06569;-3.48510;-0.23688;,
 -3.04037;-3.41979;0.28563;,
 -3.42953;-3.76989;0.34825;,
 -3.45485;-3.83520;-0.17426;,
 -3.04037;-3.41979;0.28563;,
 -6.20865;0.05184;0.00529;,
 -6.59781;-0.29826;0.06792;,
 -3.42953;-3.76989;0.34825;,
 -6.20865;0.05184;0.00529;,
 -6.23397;-0.01347;-0.51723;,
 -6.62314;-0.36356;-0.45461;,
 -6.59781;-0.29826;0.06792;,
 -3.06569;-3.48510;-0.23688;,
 -6.23397;-0.01347;-0.51723;,
 -6.62314;-0.36356;-0.45461;,
 -3.45485;-3.83520;-0.17426;,
 -3.36704;3.16661;-1.09416;,
 -6.44540;-0.29453;-0.24970;,
 -6.68399;0.00590;0.11190;,
 -3.60564;3.46704;-0.73257;,
 -6.44540;-0.29453;-0.24970;,
 -6.12570;-0.48815;0.12211;,
 -6.36430;-0.18772;0.48370;,
 -6.68399;0.00590;0.11190;,
 -6.12570;-0.48815;0.12211;,
 -3.04735;2.97299;-0.72236;,
 -3.28594;3.27342;-0.36076;,
 -6.36430;-0.18772;0.48370;,
 -3.04735;2.97299;-0.72236;,
 -3.36704;3.16661;-1.09416;,
 -3.60564;3.46704;-0.73257;,
 -3.28594;3.27342;-0.36076;,
 -6.44540;-0.29453;-0.24970;,
 -3.36704;3.16661;-1.09416;,
 -3.60564;3.46704;-0.73257;,
 -6.68399;0.00590;0.11190;,
 -2.09387;4.22700;-0.80982;,
 -5.58556;1.06880;-0.75819;,
 -5.92329;1.44466;-0.60785;,
 -2.43160;4.60286;-0.65948;,
 -5.58556;1.06880;-0.75819;,
 -5.48059;0.96101;-0.25292;,
 -5.81832;1.33687;-0.10258;,
 -5.92329;1.44466;-0.60785;,
 -5.48059;0.96101;-0.25292;,
 -1.98890;4.11921;-0.30455;,
 -2.32663;4.49507;-0.15421;,
 -5.81832;1.33687;-0.10258;,
 -1.98890;4.11921;-0.30455;,
 -2.09387;4.22700;-0.80982;,
 -2.43160;4.60286;-0.65948;,
 -2.32663;4.49507;-0.15421;,
 -5.58556;1.06880;-0.75819;,
 -2.09387;4.22700;-0.80982;,
 -2.43160;4.60286;-0.65948;,
 -5.92329;1.44466;-0.60785;,
 -0.26819;5.93758;-0.65353;,
 -4.56706;2.52907;0.17803;,
 -4.88364;2.91296;0.00386;,
 -0.58479;6.32147;-0.82770;,
 -4.56706;2.52907;0.17803;,
 -4.61333;2.71394;0.66958;,
 -4.92991;3.09782;0.49540;,
 -4.88364;2.91296;0.00386;,
 -4.61333;2.71394;0.66958;,
 -0.31447;6.12244;-0.16198;,
 -0.63106;6.50633;-0.33615;,
 -4.92991;3.09782;0.49540;,
 -0.31447;6.12244;-0.16198;,
 -0.26819;5.93758;-0.65353;,
 -0.58479;6.32147;-0.82770;,
 -0.63106;6.50633;-0.33615;,
 -4.56706;2.52907;0.17803;,
 -0.26819;5.93758;-0.65353;,
 -0.58479;6.32147;-0.82770;,
 -4.88364;2.91296;0.00386;,
 -4.71413;1.39642;0.21935;,
 -2.66252;4.68075;-0.57399;,
 -2.51532;4.65778;-0.22925;,
 -4.56691;1.37347;0.56408;,
 -2.66252;4.68075;-0.57399;,
 -2.94602;4.88692;-0.43920;,
 -2.79880;4.86397;-0.09446;,
 -2.51532;4.65778;-0.22925;,
 -2.94602;4.88692;-0.43920;,
 -4.99761;1.60260;0.35413;,
 -4.85039;1.57965;0.69887;,
 -2.79880;4.86397;-0.09446;,
 -4.99761;1.60260;0.35413;,
 -4.71413;1.39642;0.21935;,
 -4.56691;1.37347;0.56408;,
 -4.85039;1.57965;0.69887;,
 -2.66252;4.68075;-0.57399;,
 -4.71413;1.39642;0.21935;,
 -4.56691;1.37347;0.56408;,
 -2.51532;4.65778;-0.22925;,
 -0.51537;6.60146;-0.14038;,
 -2.44184;3.15608;-0.34819;,
 -2.52993;3.24036;-0.70341;,
 -0.60347;6.68573;-0.49560;,
 -2.44184;3.15608;-0.34819;,
 -2.75461;3.32715;-0.23006;,
 -2.84269;3.41143;-0.58528;,
 -2.52993;3.24036;-0.70341;,
 -2.75461;3.32715;-0.23006;,
 -0.82815;6.77252;-0.02224;,
 -0.91623;6.85680;-0.37746;,
 -2.84269;3.41143;-0.58528;,
 -0.82815;6.77252;-0.02224;,
 -0.51537;6.60146;-0.14038;,
 -0.60347;6.68573;-0.49560;,
 -0.91623;6.85680;-0.37746;,
 -2.44184;3.15608;-0.34819;,
 -0.51537;6.60146;-0.14038;,
 -0.60347;6.68573;-0.49560;,
 -2.52993;3.24036;-0.70341;,
 -2.22705;-4.08004;-0.08360;,
 0.37593;-7.02121;-0.52959;,
 0.09436;-7.26312;-0.47260;,
 -2.50861;-4.32194;-0.02661;,
 0.37593;-7.02121;-0.52959;,
 0.44383;-7.01325;-0.16031;,
 0.16226;-7.25515;-0.10332;,
 0.09436;-7.26312;-0.47260;,
 0.44383;-7.01325;-0.16031;,
 -2.15915;-4.07207;0.28568;,
 -2.44072;-4.31398;0.34267;,
 0.16226;-7.25515;-0.10332;,
 -2.15915;-4.07207;0.28568;,
 -2.22705;-4.08004;-0.08360;,
 -2.50861;-4.32194;-0.02661;,
 -2.44072;-4.31398;0.34267;,
 0.37593;-7.02121;-0.52959;,
 -2.22705;-4.08004;-0.08360;,
 -2.50861;-4.32194;-0.02661;,
 0.09436;-7.26312;-0.47260;,
 -4.61572;-0.95784;-0.38987;,
 -2.45592;-4.23491;0.08023;,
 -2.75633;-4.43381;-0.02577;,
 -4.91613;-1.15674;-0.49586;,
 -2.45592;-4.23491;0.08023;,
 -2.56901;-4.25467;0.43781;,
 -2.86942;-4.45356;0.33181;,
 -2.75633;-4.43381;-0.02577;,
 -2.56901;-4.25467;0.43781;,
 -4.72880;-0.97760;-0.03228;,
 -5.02921;-1.17650;-0.13828;,
 -2.86942;-4.45356;0.33181;,
 -4.72880;-0.97760;-0.03228;,
 -4.61572;-0.95784;-0.38987;,
 -4.91613;-1.15674;-0.49586;,
 -5.02921;-1.17650;-0.13828;,
 -2.45592;-4.23491;0.08023;,
 -4.61572;-0.95784;-0.38987;,
 -4.91613;-1.15674;-0.49586;,
 -2.75633;-4.43381;-0.02577;;
 
 172;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;3,2,10,11;,
 4;2,5,12,10;,
 4;5,7,13,12;,
 4;7,9,14,13;,
 4;11,10,15,16;,
 4;10,12,17,15;,
 4;12,13,18,17;,
 4;13,14,19,18;,
 4;16,15,20,21;,
 4;15,17,22,20;,
 4;17,18,23,22;,
 4;18,19,24,23;,
 4;21,20,25,26;,
 4;20,22,27,25;,
 4;22,23,28,27;,
 4;23,24,29,28;,
 4;26,25,30,31;,
 4;25,27,32,30;,
 4;27,28,33,32;,
 4;28,29,34,33;,
 4;31,30,35,36;,
 4;30,32,37,35;,
 4;32,33,38,37;,
 4;33,34,39,38;,
 4;36,35,40,41;,
 4;35,37,42,40;,
 4;37,38,43,42;,
 4;38,39,44,43;,
 4;41,40,45,46;,
 4;40,42,47,45;,
 4;42,43,48,47;,
 4;43,44,49,48;,
 4;46,45,50,51;,
 4;45,47,52,50;,
 4;47,48,53,52;,
 4;48,49,54,53;,
 4;51,50,55,56;,
 4;50,52,57,55;,
 4;52,53,58,57;,
 4;53,54,59,58;,
 4;56,55,60,61;,
 4;55,57,62,60;,
 4;57,58,63,62;,
 4;58,59,64,63;,
 4;61,60,65,66;,
 4;60,62,67,65;,
 4;62,63,68,67;,
 4;63,64,69,68;,
 4;66,65,70,71;,
 4;65,67,72,70;,
 4;67,68,73,72;,
 4;68,69,74,73;,
 4;71,70,75,76;,
 4;70,72,77,75;,
 4;72,73,78,77;,
 4;73,74,79,78;,
 3;76,75,80;,
 3;75,77,81;,
 3;77,78,82;,
 3;78,79,83;,
 4;84,85,86,87;,
 4;88,89,90,91;,
 4;92,93,94,95;,
 4;96,97,98,99;,
 4;96,89,100,101;,
 4;102,103,90,99;,
 4;104,105,106,107;,
 4;108,109,110,111;,
 4;112,113,114,115;,
 4;116,117,118,119;,
 4;116,109,120,121;,
 4;122,123,110,119;,
 4;124,125,126,127;,
 4;128,129,130,131;,
 4;132,133,134,135;,
 4;136,137,138,139;,
 4;136,129,140,141;,
 4;142,143,130,139;,
 4;144,145,146,147;,
 4;148,149,150,151;,
 4;152,153,154,155;,
 4;156,157,158,159;,
 4;156,149,160,161;,
 4;162,163,150,159;,
 4;164,165,166,167;,
 4;168,169,170,171;,
 4;172,173,174,175;,
 4;176,177,178,179;,
 4;176,169,180,181;,
 4;182,183,170,179;,
 4;184,185,186,187;,
 4;188,189,190,191;,
 4;192,193,194,195;,
 4;196,197,198,199;,
 4;196,189,200,201;,
 4;202,203,190,199;,
 4;204,205,206,207;,
 4;208,209,210,211;,
 4;212,213,214,215;,
 4;216,217,218,219;,
 4;216,209,220,221;,
 4;222,223,210,219;,
 4;224,225,226,227;,
 4;228,229,230,231;,
 4;232,233,234,235;,
 4;236,237,238,239;,
 4;236,229,240,241;,
 4;242,243,230,239;,
 4;244,245,246,247;,
 4;248,249,250,251;,
 4;252,253,254,255;,
 4;256,257,258,259;,
 4;256,249,260,261;,
 4;262,263,250,259;,
 4;264,265,266,267;,
 4;268,269,270,271;,
 4;272,273,274,275;,
 4;276,277,278,279;,
 4;276,269,280,281;,
 4;282,283,270,279;,
 4;284,285,286,287;,
 4;288,289,290,291;,
 4;292,293,294,295;,
 4;296,297,298,299;,
 4;296,289,300,301;,
 4;302,303,290,299;,
 4;304,305,306,307;,
 4;308,309,310,311;,
 4;312,313,314,315;,
 4;316,317,318,319;,
 4;316,309,320,321;,
 4;322,323,310,319;,
 4;324,325,326,327;,
 4;328,329,330,331;,
 4;332,333,334,335;,
 4;336,337,338,339;,
 4;336,329,340,341;,
 4;342,343,330,339;,
 4;344,345,346,347;,
 4;348,349,350,351;,
 4;352,353,354,355;,
 4;356,357,358,359;,
 4;356,349,360,361;,
 4;362,363,350,359;,
 4;364,365,366,367;,
 4;368,369,370,371;,
 4;372,373,374,375;,
 4;376,377,378,379;,
 4;376,369,380,381;,
 4;382,383,370,379;,
 4;384,385,386,387;,
 4;388,389,390,391;,
 4;392,393,394,395;,
 4;396,397,398,399;,
 4;396,389,400,401;,
 4;402,403,390,399;,
 4;404,405,406,407;,
 4;408,409,410,411;,
 4;412,413,414,415;,
 4;416,417,418,419;,
 4;416,409,420,421;,
 4;422,423,410,419;,
 4;424,425,426,427;,
 4;428,429,430,431;,
 4;432,433,434,435;,
 4;436,437,438,439;,
 4;436,429,440,441;,
 4;442,443,430,439;;
 
 MeshMaterialList {
  3;
  172;
  1,
  1,
  1,
  1,
  2,
  2,
  2,
  2,
  1,
  1,
  1,
  1,
  2,
  2,
  2,
  2,
  1,
  1,
  1,
  1,
  2,
  2,
  2,
  2,
  1,
  1,
  1,
  1,
  2,
  2,
  2,
  2,
  1,
  1,
  1,
  1,
  2,
  2,
  2,
  2,
  1,
  1,
  1,
  1,
  2,
  2,
  2,
  2,
  1,
  1,
  1,
  1,
  2,
  2,
  2,
  2,
  1,
  1,
  1,
  1,
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
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   1.000000;
   0.000000;0.000000;0.000000;;
   0.350000;0.350000;0.350000;;
  }
  Material {
   0.696471;0.696471;0.696471;1.000000;;
   50.000000;
   0.250000;0.250000;0.250000;;
   0.435294;0.435294;0.435294;;
  }
  Material {
   0.564706;0.564706;0.564706;1.000000;;
   1.000000;
   0.150000;0.150000;0.150000;;
   0.176471;0.176471;0.176471;;
  }
 }
 MeshNormals {
  189;
  -0.725485;-0.646858;-0.235044;,
  -0.725485;0.646858;-0.235044;,
  0.725485;0.646858;-0.235044;,
  -0.714279;-0.621792;-0.321215;,
  -0.676012;0.614776;-0.406274;,
  0.714279;0.621793;-0.321215;,
  -0.741475;-0.589032;-0.321336;,
  -0.646210;0.645927;-0.406438;,
  0.741475;0.589032;-0.321336;,
  -0.752718;-0.547955;-0.364913;,
  -0.608736;0.659694;-0.440732;,
  0.752718;0.547955;-0.364913;,
  -0.777064;-0.512751;-0.365044;,
  -0.576052;0.688315;-0.440893;,
  0.777064;0.512751;-0.365044;,
  -0.781283;-0.470343;-0.410334;,
  -0.538802;0.697889;-0.471850;,
  0.781283;0.470343;-0.410334;,
  -0.802604;-0.432832;-0.410467;,
  -0.503422;0.723729;-0.471998;,
  0.802604;0.432832;-0.410467;,
  -0.801472;-0.391305;-0.452243;,
  -0.467634;0.730454;-0.497750;,
  0.801472;0.391305;-0.452243;,
  -0.819569;-0.351669;-0.452366;,
  -0.773835;-0.037298;-0.632288;,
  -0.429801;0.753251;-0.497880;,
  0.773835;0.037299;-0.632288;,
  -0.815712;-0.312780;-0.486603;,
  -0.396054;0.758479;-0.517544;,
  0.815712;0.312780;-0.486603;,
  -0.830375;-0.271285;-0.486706;,
  -0.746073;0.026569;-0.665334;,
  -0.356076;0.777978;-0.517649;,
  0.746073;-0.026569;-0.665334;,
  -0.826081;-0.235620;-0.511931;,
  -0.324330;0.782812;-0.531051;,
  0.826081;0.235620;-0.511931;,
  -0.565305;-0.396646;-0.723258;,
  -0.719879;0.088102;-0.688485;,
  0.045798;0.692902;-0.719576;,
  0.719879;-0.088102;-0.688485;,
  -0.833710;-0.159757;-0.528587;,
  -0.252362;0.804048;-0.538349;,
  0.833711;0.159757;-0.528587;,
  -0.096369;-0.675195;-0.731317;,
  -0.695739;0.148690;-0.702736;,
  0.096370;0.675195;-0.731317;,
  0.695739;-0.148690;-0.702736;,
  -0.842214;-0.099248;-0.529929;,
  -0.194791;0.825368;-0.529929;,
  0.842214;0.099248;-0.529929;,
  -0.430292;0.301294;-0.850924;,
  0.725485;-0.646858;-0.235044;,
  0.676012;-0.614776;-0.406274;,
  0.646210;-0.645927;-0.406438;,
  0.608736;-0.659694;-0.440732;,
  0.576052;-0.688315;-0.440893;,
  0.538802;-0.697889;-0.471850;,
  0.503422;-0.723729;-0.471999;,
  0.467634;-0.730454;-0.497750;,
  0.819569;0.351669;-0.452366;,
  0.429801;-0.753251;-0.497880;,
  -0.460495;0.855978;-0.235044;,
  0.460495;-0.855978;-0.235044;,
  0.396054;-0.758478;-0.517544;,
  0.830375;0.271285;-0.486706;,
  0.356076;-0.777978;-0.517649;,
  -0.384139;0.892856;-0.235045;,
  0.384139;-0.892856;-0.235045;,
  0.324330;-0.782812;-0.531051;,
  -0.304860;0.922938;-0.235044;,
  0.951718;0.197453;-0.235044;,
  0.304860;-0.922938;-0.235044;,
  0.252362;-0.804048;-0.538349;,
  -0.965305;-0.113753;-0.235045;,
  -0.223260;0.945996;-0.235045;,
  0.965305;0.113754;-0.235045;,
  0.223260;-0.945996;-0.235045;,
  0.194791;-0.825367;-0.529930;,
  0.430292;-0.301293;-0.850924;,
  0.150942;0.079959;-0.985304;,
  0.734804;-0.677173;0.038727;,
  -0.150942;-0.079960;0.985304;,
  -0.734802;0.677175;-0.038727;,
  0.666933;0.727486;0.161136;,
  -0.666933;-0.727486;-0.161135;,
  -0.129891;-0.062235;-0.989573;,
  0.668637;-0.743587;0.001635;,
  0.129891;0.062236;0.989573;,
  -0.668633;0.743590;-0.001640;,
  0.750885;0.645799;-0.138255;,
  -0.750886;-0.645799;0.138253;,
  0.187693;0.186154;-0.964426;,
  0.766657;-0.641556;0.025371;,
  -0.187693;-0.186154;0.964426;,
  -0.766656;0.641557;-0.025370;,
  0.614013;0.744144;0.263130;,
  -0.614012;-0.744144;-0.263133;,
  -0.365313;-0.057429;-0.929112;,
  0.617111;-0.786388;0.027719;,
  0.323809;0.116973;0.938864;,
  -0.617110;0.786389;-0.027719;,
  0.765769;0.552677;-0.328856;,
  -0.808273;-0.490898;0.325137;,
  -0.369707;0.245833;-0.896037;,
  0.496703;0.867778;-0.015726;,
  0.369707;-0.245834;0.896037;,
  -0.496702;-0.867779;0.015725;,
  -0.792251;0.418960;0.443634;,
  0.792253;-0.418958;-0.443632;,
  0.282932;-0.141799;-0.948600;,
  0.742662;0.665352;0.075888;,
  -0.282931;0.141798;0.948601;,
  -0.742657;-0.665358;-0.075890;,
  -0.647992;0.701590;-0.296442;,
  0.647996;-0.701588;0.296439;,
  -0.458950;-0.127607;-0.879250;,
  0.580829;-0.791960;-0.188246;,
  0.458952;0.127607;0.879250;,
  -0.580835;0.791957;0.188242;,
  0.672305;0.597094;-0.437589;,
  -0.672306;-0.597095;0.437587;,
  0.214941;-0.180096;-0.959878;,
  -0.566544;-0.823567;0.027656;,
  -0.214941;0.180096;0.959878;,
  0.566546;0.823566;-0.027658;,
  0.795506;-0.537868;0.279048;,
  -0.795506;0.537868;-0.279050;,
  0.551891;-0.527203;-0.646122;,
  -0.528997;-0.820283;0.217481;,
  -0.551891;0.527205;0.646120;,
  0.528991;0.820288;-0.217479;,
  0.644661;-0.221763;0.731596;,
  -0.644661;0.221764;-0.731596;,
  0.542810;0.516830;-0.662000;,
  0.719712;-0.689067;0.084853;,
  -0.542811;-0.516832;0.661997;,
  -0.719715;0.689065;-0.084853;,
  0.406258;0.528481;0.745427;,
  -0.406260;-0.528483;-0.745425;,
  -0.048038;-0.123877;-0.991134;,
  0.672899;-0.737334;0.059545;,
  0.048042;0.123880;0.991134;,
  -0.672897;0.737336;-0.059544;,
  0.738170;0.664076;-0.118778;,
  -0.738169;-0.664077;0.118780;,
  -0.606400;0.367262;-0.705264;,
  -0.653799;-0.735104;0.179355;,
  0.606402;-0.367264;0.705261;,
  0.653802;0.735101;-0.179356;,
  0.452565;-0.569861;-0.685889;,
  -0.452568;0.569861;0.685887;,
  -0.199106;0.204463;-0.958411;,
  -0.741595;-0.670759;0.010968;,
  0.199106;-0.204463;0.958411;,
  0.741592;0.670761;-0.010969;,
  0.640623;-0.712933;-0.285182;,
  -0.640625;0.712932;0.285180;,
  0.093865;-0.346121;-0.933482;,
  -0.794786;-0.588902;0.146661;,
  -0.093864;0.346120;0.933483;,
  0.794781;0.588909;-0.146662;,
  0.625306;-0.709217;0.325582;,
  -0.625307;0.709215;-0.325584;,
  0.750790;-0.555370;-0.357600;,
  0.525882;0.833589;-0.169049;,
  -0.750793;0.555371;0.357594;,
  -0.525883;-0.833582;0.169082;,
  -0.408398;0.034781;-0.912141;,
  0.408405;-0.034787;0.912138;,
  0.836632;-0.448862;-0.313957;,
  -0.501435;-0.861484;-0.080058;,
  -0.836636;0.448865;0.313942;,
  0.501417;0.861495;0.080047;,
  0.250246;-0.197100;0.947908;,
  -0.250236;0.197094;-0.947911;,
  -0.185673;-0.015343;-0.982492;,
  0.636572;-0.764633;-0.100560;,
  0.185672;0.015342;0.982492;,
  -0.636575;0.764633;0.100548;,
  0.729264;0.667925;-0.148492;,
  -0.729263;-0.667925;0.148495;,
  0.297120;0.059113;-0.953008;,
  0.519110;-0.846608;0.117388;,
  -0.297120;-0.059113;0.953008;,
  -0.519110;0.846609;-0.117381;,
  0.783128;0.556065;0.278392;,
  -0.783128;-0.556066;-0.278390;;
  172;
  4;0,0,3,3;,
  4;1,1,4,4;,
  4;2,2,5,5;,
  4;53,53,54,54;,
  4;3,3,6,6;,
  4;4,4,7,7;,
  4;5,5,8,8;,
  4;54,54,55,55;,
  4;6,6,9,9;,
  4;7,7,10,10;,
  4;8,8,11,11;,
  4;55,55,56,56;,
  4;9,9,12,12;,
  4;10,10,13,13;,
  4;11,11,14,14;,
  4;56,56,57,57;,
  4;12,12,15,15;,
  4;13,13,16,16;,
  4;14,14,17,17;,
  4;57,57,58,58;,
  4;15,15,18,18;,
  4;16,16,19,19;,
  4;17,17,20,20;,
  4;58,58,59,59;,
  4;18,18,21,21;,
  4;19,19,22,22;,
  4;20,20,23,23;,
  4;59,59,60,60;,
  4;21,21,25,24;,
  4;22,22,26,25;,
  4;23,23,27,61;,
  4;60,60,62,27;,
  4;24,25,28,28;,
  4;63,26,29,29;,
  4;61,27,30,30;,
  4;64,62,65,65;,
  4;28,28,32,31;,
  4;29,29,33,32;,
  4;30,30,34,66;,
  4;65,65,67,34;,
  4;31,32,35,35;,
  4;68,33,36,36;,
  4;66,34,37,37;,
  4;69,67,70,70;,
  4;35,35,39,38;,
  4;36,36,40,39;,
  4;37,37,41,40;,
  4;70,70,38,41;,
  4;38,39,42,42;,
  4;71,40,43,43;,
  4;72,41,44,44;,
  4;73,73,74,74;,
  4;42,42,46,45;,
  4;43,43,47,46;,
  4;44,44,48,47;,
  4;74,74,45,48;,
  4;75,46,49,49;,
  4;76,47,50,50;,
  4;77,48,51,51;,
  4;78,45,79,79;,
  3;49,49,52;,
  3;50,50,52;,
  3;51,51,80;,
  3;79,79,80;,
  4;81,81,81,81;,
  4;82,82,82,82;,
  4;83,83,83,83;,
  4;84,84,84,84;,
  4;85,85,85,85;,
  4;86,86,86,86;,
  4;87,87,87,87;,
  4;88,88,88,88;,
  4;89,89,89,89;,
  4;90,90,90,90;,
  4;91,91,91,91;,
  4;92,92,92,92;,
  4;93,93,93,93;,
  4;94,94,94,94;,
  4;95,95,95,95;,
  4;96,96,96,96;,
  4;97,97,97,97;,
  4;98,98,98,98;,
  4;99,99,99,99;,
  4;100,100,100,100;,
  4;101,101,101,101;,
  4;102,102,102,102;,
  4;103,103,103,103;,
  4;104,104,104,104;,
  4;105,105,105,105;,
  4;106,106,106,106;,
  4;107,107,107,107;,
  4;108,108,108,108;,
  4;109,109,109,109;,
  4;110,110,110,110;,
  4;111,111,111,111;,
  4;112,112,112,112;,
  4;113,113,113,113;,
  4;114,114,114,114;,
  4;115,115,115,115;,
  4;116,116,116,116;,
  4;117,117,117,117;,
  4;118,118,118,118;,
  4;119,119,119,119;,
  4;120,120,120,120;,
  4;121,121,121,121;,
  4;122,122,122,122;,
  4;123,123,123,123;,
  4;124,124,124,124;,
  4;125,125,125,125;,
  4;126,126,126,126;,
  4;127,127,127,127;,
  4;128,128,128,128;,
  4;129,129,129,129;,
  4;130,130,130,130;,
  4;131,131,131,131;,
  4;132,132,132,132;,
  4;133,133,133,133;,
  4;134,134,134,134;,
  4;135,135,135,135;,
  4;136,136,136,136;,
  4;137,137,137,137;,
  4;138,138,138,138;,
  4;139,139,139,139;,
  4;140,140,140,140;,
  4;141,141,141,141;,
  4;142,142,142,142;,
  4;143,143,143,143;,
  4;144,144,144,144;,
  4;145,145,145,145;,
  4;146,146,146,146;,
  4;147,147,147,147;,
  4;148,148,148,148;,
  4;149,149,149,149;,
  4;150,150,150,150;,
  4;151,151,151,151;,
  4;152,152,152,152;,
  4;153,153,153,153;,
  4;154,154,154,154;,
  4;155,155,155,155;,
  4;156,156,156,156;,
  4;157,157,157,157;,
  4;158,158,158,158;,
  4;159,159,159,159;,
  4;160,160,160,160;,
  4;161,161,161,161;,
  4;162,162,162,162;,
  4;163,163,163,163;,
  4;164,164,164,164;,
  4;165,165,165,165;,
  4;166,166,166,166;,
  4;167,167,167,167;,
  4;168,168,168,168;,
  4;169,169,169,169;,
  4;170,170,170,170;,
  4;171,171,171,171;,
  4;172,172,172,172;,
  4;173,173,173,173;,
  4;174,174,174,174;,
  4;175,175,175,175;,
  4;176,176,176,176;,
  4;177,177,177,177;,
  4;178,178,178,178;,
  4;179,179,179,179;,
  4;180,180,180,180;,
  4;181,181,181,181;,
  4;182,182,182,182;,
  4;183,183,183,183;,
  4;184,184,184,184;,
  4;185,185,185,185;,
  4;186,186,186,186;,
  4;187,187,187,187;,
  4;188,188,188,188;;
 }
 MeshTextureCoords {
  444;
  0.000000;1.000000;,
  0.250000;1.000000;,
  0.250000;0.937500;,
  0.000000;0.937500;,
  0.500000;1.000000;,
  0.500000;0.937500;,
  0.750000;1.000000;,
  0.750000;0.937500;,
  1.000000;1.000000;,
  1.000000;0.937500;,
  0.250000;0.875000;,
  0.000000;0.875000;,
  0.500000;0.875000;,
  0.750000;0.875000;,
  1.000000;0.875000;,
  0.250000;0.812500;,
  0.000000;0.812500;,
  0.500000;0.812500;,
  0.750000;0.812500;,
  1.000000;0.812500;,
  0.250000;0.750000;,
  0.000000;0.750000;,
  0.500000;0.750000;,
  0.750000;0.750000;,
  1.000000;0.750000;,
  0.250000;0.687500;,
  0.000000;0.687500;,
  0.500000;0.687500;,
  0.750000;0.687500;,
  1.000000;0.687500;,
  0.250000;0.625000;,
  0.000000;0.625000;,
  0.500000;0.625000;,
  0.750000;0.625000;,
  1.000000;0.625000;,
  0.250000;0.562500;,
  0.000000;0.562500;,
  0.500000;0.562500;,
  0.750000;0.562500;,
  1.000000;0.562500;,
  0.250000;0.500000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.750000;0.500000;,
  1.000000;0.500000;,
  0.250000;0.437500;,
  0.000000;0.437500;,
  0.500000;0.437500;,
  0.750000;0.437500;,
  1.000000;0.437500;,
  0.250000;0.375000;,
  0.000000;0.375000;,
  0.500000;0.375000;,
  0.750000;0.375000;,
  1.000000;0.375000;,
  0.250000;0.312500;,
  0.000000;0.312500;,
  0.500000;0.312500;,
  0.750000;0.312500;,
  1.000000;0.312500;,
  0.250000;0.250000;,
  0.000000;0.250000;,
  0.500000;0.250000;,
  0.750000;0.250000;,
  1.000000;0.250000;,
  0.250000;0.187500;,
  0.000000;0.187500;,
  0.500000;0.187500;,
  0.750000;0.187500;,
  1.000000;0.187500;,
  0.250000;0.125000;,
  0.000000;0.125000;,
  0.500000;0.125000;,
  0.750000;0.125000;,
  1.000000;0.125000;,
  0.250000;0.062500;,
  0.000000;0.062500;,
  0.500000;0.062500;,
  0.750000;0.062500;,
  1.000000;0.062500;,
  0.125000;0.000000;,
  0.250000;0.000000;,
  0.375000;0.000000;,
  0.500000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}