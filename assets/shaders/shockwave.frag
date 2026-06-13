// V2 #33: Victory shockwave - vong song sang lan ra tu tam chien thang.
// Ve procedural (khong dung texture), dua tren khoang cach toi u_center.
// GLSL legacy (khong #version) de tuong thich rong voi SFML / GPU cu.

uniform float u_time;        // giay tu luc co nguoi thang
uniform vec2  u_resolution;  // kich thuoc khung ve (px) - o day la 1280x720 logic
uniform vec2  u_center;      // tam song (px, he toa do SFML goc tren-trai)

void main() {
    // gl_FragCoord goc duoi-trai -> lat y de khop he toa do SFML (goc tren-trai)
    vec2 frag = vec2(gl_FragCoord.x, u_resolution.y - gl_FragCoord.y);
    float dist = distance(frag, u_center);

    // Vong song chinh: ban kinh lan ra theo thoi gian
    float radius = u_time * 1150.0;          // toc do lan (px/s)
    float thickness = 75.0;                  // do day vong
    float ring = 1.0 - smoothstep(0.0, thickness, abs(dist - radius));

    // Vong thu 2 (tre 0.12s) -> cam giac day song
    float radius2 = max(0.0, u_time - 0.12) * 1150.0;
    float ring2 = 1.0 - smoothstep(0.0, thickness * 0.7, abs(dist - radius2));

    // Tat dan trong 0.8s
    float life = clamp(1.0 - u_time / 0.8, 0.0, 1.0);
    float intensity = (ring + ring2 * 0.6) * life;

    vec3 color = vec3(1.0, 0.92, 0.65);      // vang am (warm gold)
    // Alpha mang cuong do; ket hop BlendAdd ben C++ -> lam sang vung vong song
    gl_FragColor = vec4(color, intensity * 0.8);
}
