include <BOSL2/std.scad>

/*
----------parameters------------------------------------------------------------
*/

/*[Screw DIN 7981, DIN-ISO 7049]*/
// outer thread diameter
screw_d = 2.9; //[2.9,3.9]
// head diameter
screw_dk = 5.6; //[5.6,7.5]
// lenght
screw_l = 15; //[13,16]

/*[Quality]*/

// roundness of circle
$fn = 10; //[3,5,8,13,21,34,55,89,144,233]

// quality_offset (additional offset, for better results in preview
qo = 0.001;

/*[measurtements]*/

// x lenght
case_x = 61;
// y lenght
case_y = 48;
// z lenght
case_z = 33;
// outer radius of case corner
case_r = 4;
// outer radius of case bottom corner
case_br = 2;
// wall size
case_w = 2;

/*
----------modules-&-functions---------------------------------------------------
*/

module cube_rcr(x, y, z, r, br = 0.001)
{
    translate([ r, r, br ]) linear_extrude(z - br) offset(r) square([ x - r * 2, y - r * 2 ]);
    for (i = [[r, r, br], [r, y - r, br], [x - r, y - r, br], [x - r, r, br]])
        translate(i)
        {
            rotate_extrude()
            {
                translate([ 0, -br ]) square([ r - br, br * 2 ]);
                translate([ r - br, 0, 0 ]) circle(r = br);
            }
        }
    translate([ r, br, br ]) rotate([ 0, 90, 0 ]) linear_extrude(x - r - r)
    {
        translate([ -br, 0 ]) square([ br + br, y - br - br ]);
        translate([ 0, y - br - br, 0 ]) circle(r = br);
        circle(r = br);
    }
    translate([ x - br, r, br ]) rotate([ 0, 90, 90 ]) linear_extrude(y - r - r)
    {
        translate([ -br, 0 ]) square([ br + br, x - br - br ]);
        translate([ 0, x - br - br, 0 ]) circle(r = br);
        circle(r = br);
    }
};

module screwhole()
{
    difference()
    {
        cylinder(h = screw_l - case_w, r = case_r - case_w * 0.66);
        cylinder(h = screw_l - case_w + qo, r = screw_d / 2 * 0.8);
    };
};

module print_body()
{
    difference()
    {
        union()
        {
            difference()
            {
                cube_rcr(case_x, case_y, case_z, case_r, case_br);
                translate([ case_w, case_w, case_w ])
                    cube_rcr(case_x - 2 * case_w, case_y - 2 * case_w, case_z - case_w + qo, case_r - case_w, 1);
            };
            if ((case_x >= case_r * 3) && (case_y >= case_r * 3))
            {
                for (p = [[case_r, case_r, case_z - screw_l + case_w],
                          [case_x - case_r, case_r, case_z - screw_l + case_w],
                          [case_r, case_y - case_r, case_z - screw_l + case_w],
                          [case_x - case_r, case_y - case_r, case_z - screw_l + case_w]])
                    translate(p) screwhole();
            }
            else if ((case_x >= case_r * 3) && (case_y < case_r * 3))
            {
                for (p = [[case_r, case_y / 2, case_z - screw_l + case_w],
                          [case_x - case_r, case_y / 2, case_z - screw_l + case_w]])
                    translate(p) screwhole();
            }
            else if ((case_x < case_r * 3) && (case_y >= case_r * 3))
            {
                for (p = [[case_x / 2, case_r, case_z - screw_l + case_w],
                          [case_x / 2, case_y - case_r, case_z - screw_l + case_w]])
                    translate(p) screwhole();
            }
            else if ((case_x < case_r * 3) && (case_y < case_r * 3))
            {
            };
        };
        difference()
        {
            translate([ -1, -1, case_z - case_w ]) cube_rcr(case_x + 2, case_y + 2, case_w + qo, case_r);
            translate([ case_w * 0.6, case_w * 0.6, case_z - case_w ])
                cube_rcr(case_x - case_w * 1.2, case_y - case_w * 1.2, case_w, case_r * 0.8);
        };
    };
};

module print_cover()
{
    difference()
    {
        cube_rcr(case_x, case_y, case_w, case_r);
        translate([ case_w * 0.4, case_w * 0.4, -qo ])
            cube_rcr(case_x - case_w * 0.8, case_y - case_w * 0.8, case_w + 2 * qo, case_r * 0.8);
    };
    translate([ 0, 0, case_w ])
    {
        difference()
        {
            cube_rcr(case_x, case_y, case_w, case_r);
            union()
            {
                if ((case_x >= case_r * 3) && (case_y >= case_r * 3))
                {
                    translate([ 0, 0, -qo ])
                    {
                        for (p = [[case_r, case_r, 0], [case_x - case_r, case_r, 0], [case_r, case_y - case_r, 0],
                                  [case_x - case_r, case_y - case_r, 0]])
                            screw_hole_with_countersink(p);
                    };
                }
                else if ((case_x >= case_r * 3) && (case_y < case_r * 3))
                {
                    for (p = [[case_r, case_y / 2, 0], [case_x - case_r, case_y / 2, 0]])
                        screw_hole_with_countersink(p);
                }
                else if ((case_x < case_r * 3) && (case_y >= case_r * 3))
                {
                    for (p = [[case_x / 2, case_r, 0], [case_x / 2, case_y - case_r, 0]])
                        screw_hole_with_countersink(p);
                }
                else if ((case_x < case_r * 3) && (case_y < case_r * 3))
                {
                }
            }
        }
    }
}

module screw_hole_with_countersink(p)
{
    translate(p) cylinder(h = case_w, d = ceil(screw_d));
    translate([ p[0], p[1], case_w - 0.25 ]) cylinder(h = case_w, d = screw_dk);
}

module buildPyramidalExtrude(height, maxOffset, nSlices)
{
    heightIncrement = height / nSlices;
    offsetIncrement = maxOffset / (nSlices - 1);
    for (i = [1:nSlices])
        linear_extrude(height = i * heightIncrement) offset(r = maxOffset - (i - 1) * offsetIncrement) children();
}

// top part
union()
{
    difference()
    {
        print_body();

        translate([ 3, 5, 2 ]) rotate([ 0, 180, 0 ]) translate([ -57, 0, -30 ]) union()
        {

            translate([ 26.5, 14.5, 32 ]) cuboid([ 28, 17, 4 ], rounding = 2);

            translate([ 9.5, 37, 17 ]) translate([ 0, 0, -60 ]) cube([ 38, 200, 67 ]);
            translate([ -100, 16, -2 ]) cube([ 130, 12, 8 ]);
        }
    }

    translate([ 3, 5, 2 ]) rotate([ 0, 180, 0 ]) translate([ -57, 0, -30 ])
    {
        translate([ 7.5, 4.2, 17 ]) screwhole();
        translate([ 52.7, 4.2, 17 ]) screwhole();
        translate([ 52.1, 29.2, 17 ]) screwhole();
        translate([ 4.8, 29.2, 17 ]) screwhole();
    }
}

// bottom part
difference()
{
    union()
    {
        translate([ 0, 0, 31 ]) print_cover();
        translate([ 12.6, 46, 14 ]) cube([ 37.85, 2, 19 ]);
    }
    translate([ 3, 5, 2 ]) rotate([ 0, 180, 0 ]) translate([ -57, 0, -30 ]) translate([ -100, 16, -2 ])
        cube([ 103, 12, 7 ]);
}