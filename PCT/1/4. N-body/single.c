#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <omp.h>

struct particle {
    float x, y, z;
};

const float G = 6.67e-11;
void calculate_forces(struct particle *p, struct particle *f, float *m, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            float dist = sqrtf(
                powf(p[i].x - p[j].x, 2) + powf(p[i].y - p[j].y, 2) +
                powf(p[i].z - p[j].z, 2));
            float mag = (G * m[i] * m[j]) / powf(dist, 2);
            struct particle dir = {.x = p[j].x - p[i].x,
                                   .y = p[j].y - p[i].y,
                                   .z = p[j].z - p[i].z};
            f[i].x += mag * dir.x / dist;
            f[i].y += mag * dir.y / dist;
            f[i].z += mag * dir.z / dist;
            f[j].x -= mag * dir.x / dist;
            f[j].y -= mag * dir.y / dist;
            f[j].z -= mag * dir.z / dist;
        }
    }
}

void move_particles(
    struct particle *p,
    struct particle *f,
    struct particle *v,
    float *m,
    int n,
    double dt) {
    for (int i = 0; i < n; i++) {
        struct particle dv = {
            .x = f[i].x / m[i] * dt,
            .y = f[i].y / m[i] * dt,
            .z = f[i].z / m[i] * dt,
        };
        struct particle dp = {
            .x = (v[i].x + dv.x / 2) * dt,
            .y = (v[i].y + dv.y / 2) * dt,
            .z = (v[i].z + dv.z / 2) * dt,
        };
        v[i].x += dv.x;
        v[i].y += dv.y;
        v[i].z += dv.z;
        p[i].x += dp.x;
        p[i].y += dp.y;
        p[i].z += dp.z;
        f[i].x = f[i].y = f[i].z = 0;
    }
}

int main(int argc, char *argv[]) {
    double ttotal, tinit = 0, tforces = 0, tmove = 0;
    ttotal = omp_get_wtime();
    int n = (argc > 1) ? atoi(argv[1]) : 10;
    tinit = -omp_get_wtime();

    struct particle *p = malloc(sizeof(*p) * n); // Положение частицы
    struct particle *f = malloc(sizeof(*f) * n); // Сила, действующая на каждую частицу
    struct particle *v = malloc(sizeof(*v) * n); // Скорость частицы
    float *m = malloc(sizeof(*m) * n); 
              // Масса частицы
    for (int i = 0; i < n; i++) {
        p[i].x = rand() / (float)RAND_MAX - 0.5;
        p[i].y = rand() / (float)RAND_MAX - 0.5;
        p[i].z = rand() / (float)RAND_MAX - 0.5;
        v[i].x = rand() / (float)RAND_MAX - 0.5;
        v[i].y = rand() / (float)RAND_MAX - 0.5;
        v[i].z = rand() / (float)RAND_MAX - 0.5;
        m[i] = rand() / (float)RAND_MAX * 10 + 0.01;
        f[i].x = f[i].y = f[i].z = 0;
    }
    tinit += omp_get_wtime();

    double dt = 1e-5;
    for (double t = 0; t <= 1; t += dt) { // Цикл по времени (модельному)
        tforces -= omp_get_wtime();
        calculate_forces(p, f, m, n); // Вычисление сил –O(N^2)
        tforces += omp_get_wtime();
        tmove -= omp_get_wtime();
        move_particles(p, f, v, m, n, dt); // Перемещение телO(N)
        tmove += omp_get_wtime();
    }
    ttotal = omp_get_wtime() - ttotal;
    printf("# NBody (n=%d)\n", n);
    printf(
        "# Elapsed time (sec): ttotal %.6f, tinit %.6f, tforces %.6f, tmove %.6f\n",
        ttotal,
        tinit,
        tforces,
        tmove);

    free(m);
    free(v);
    free(f);
    free(p);
    return 0;
}

/*

# This file is called   force.dat
# Force-Deflection data for a beam and a bar
# Linear    sig        1        2       3       4
2           1.19       1.19     1.16    0.95    1.76
4           1.11       1.54     1.49    1.59    2.88
6           0.86       2.02     1.90    1.97    3.20
8           0.66       1.88     1.83    1.76    2,97 


# Scale font and line width (dpi) by changing the size! It will always display stretched.
set terminal svg size 600,400 enhanced fname 'arial'  fsize 10 butt solid
set output 'out.svg'

# Key means label...
set key inside top left
set xlabel 'Threads'
set ylabel 'Speedup'
set title ''
plot  "data.txt" using 1:1 title 'Lienar speedup' with lines, "data.txt" using 1:2 title 'critical' with linespoints, "data.txt" using 1:3 title '6 atomic' with linespoints, "data.txt" using 1:4 title 'n locks' with linespoints, "data.txt" using 1:5 title 'extra calculations' with linespoints, "data.txt" using 1:6 title 'local vector' with linespoints
*/
