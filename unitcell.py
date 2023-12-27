# Google Bardによる翻訳 (かなりおかしいけど)
import math


def push(jx, jy, jz, t, r):
    """
    既存のベクトルと素かどうかをチェックしろ

    Args:
      jx: x成分
      jy: y成分
      jz: z成分
      t: t成分
      r: 長さ
    """

    for i in range(nstack):
        if jx * stackx[i] + jy * stacky[i] + jz * stackz[i] == 0:
            return

    stackx.append(jx)
    stacky.append(jy)
    stackz.append(jz)
    stackt.append(t)
    stackr.append(r)
    nstack += 1


def main():
    bx, by, bz = 1.0, 1.0, 1.0
    ix, iy, iz = 1, 1, 1
    range = 10
    tolerance = 0.02
    ri = math.sqrt(ix**2 + iy**2 + iz**2)

    for jx in range(-range, range + 1):
        for jy in range(-range, range + 1):
            for jz in range(-range, range + 1):
                rjx = jx * bx
                rjy = jy * by
                rjz = jz * bz
                rj = math.sqrt(rjx**2 + rjy**2 + rjz**2)
                if rj != 0:
                    tj = abs(ix * rjx + iy * rjy + iz * rjz) / (ri * rj)
                    if tj < tolerance:
                        push(jx, jy, jz, tj, rj)

    for j in range(nstack):
        rjx = stackx[j] * bx
        rjy = stacky[j] * by
        rjz = stackz[j] * bz
        tj = stackt[j]
        rj = stackr[j]
        for k in range(j + 1, nstack):
            rkx = stackx[k] * bx
            rky = stacky[k] * by
            rkz = stackz[k] * bz
            tk = stackt[k]
            rk = stackr[k]
            tjk = abs(rkx * rjx + rky * rjy + rkz * rjz) / (rk * rj)
            if tjk < tolerance:
                x = rky * rjz - rkz * rjy
                y = rkz * rjx - rkx * rjz
                z = rkx * rjy - rky * rjx
                v = abs(x * ix + y * iy + z * iz)
                print(
                    int(math.rint(v / (bx * by * bz))),
                    v / (ri * rj * rk),
                    ix,
                    iy,
                    iz,
                    stackx[j],
                    stacky[j],
                    stackz[j],
                    stackx[k],
                    stacky[k],
                    stackz[k],
                )


if __name__ == "__main__":
    main()
