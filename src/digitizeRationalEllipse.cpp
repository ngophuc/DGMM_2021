//
// digitizeRationalEllipse.cpp
//
// Give a implicit function of a rational ellipse, this program calculates the digitization of the region
// where the function values are negative
//
// Created by Yukiko Kenmochi on 09/11/20.
//

#include <iostream>
#include <string>
#include <sstream>
#include <gmpxx.h>
#include <cstdio>
#include <ratio>

#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/images/ImageSelector.h"
#include "DGtal/io/writers/GenericWriter.h"

using namespace std;
using namespace DGtal;
using namespace Z2i;

class RationalEllipse {
public:
    // (x-center_x)^2/a^2 + (y-center_y)^2/b^2 = 1
    mpq_class center_x;
    mpq_class center_y;
    mpq_class a;            // width 2a
    mpq_class b;            // height 2b

    void set(mpq_class cx, mpq_class cy, mpq_class a, mpq_class b);
    bool isInside(mpq_class x, mpq_class y);
    Point getLowerBound();
    Point getUpperBound();
};

void RationalEllipse::set(mpq_class cx, mpq_class cy, mpq_class width, mpq_class height) {
    this->center_x = cx;
    this->center_y = cy;
    this->a = width;
    this->b = height;
}

bool RationalEllipse::isInside(mpq_class x, mpq_class y) {

    mpq_class x_num = (x-this->center_x)*(x-this->center_x);
    mpq_class x_den = (this->a)*(this->a);
    mpq_class y_num = (y-this->center_y)*(y-this->center_y);
    mpq_class y_den = (this->b)*(this->b);
    mpq_class dis = x_num/x_den + y_num/y_den;

    if ( dis <= 1 )
        return true;
    else
        return false;
}

Point RationalEllipse::getLowerBound() {

    mpq_class x = this->center_x - this->a;
    mpq_class y = this->center_y - this->a;

    if ( this->a < this->b ) {
        x = this->center_x - this->b;
        y = this->center_y - this->b;
    }

    return Point((int)x.get_d()-1, (int)y.get_d()-1 );
}

Point RationalEllipse::getUpperBound() {

    mpq_class x = this->center_x + this->a;
    mpq_class y = this->center_y + this->a;

    if ( this-> a < this->b ){
        x = this->center_x + this->b;
        y = this->center_y + this->b;
    }

    return Point((int)x.get_d()+1, (int)y.get_d()+1 );
}

void usage(int argc, char** argv) {
    std::cerr << "Usage: " << argv[0] << " <ellipse center x num> <x den> <ellipse center y num> <y den> <ellipse width num> <width den> <ellipse height num> <height den> (<Ra> <Rb> <Rc> <Tx_n> <Tx_d> <Ty_n> <Ty_d>) <output image file>" << std::endl;
    std::cerr << "  The ellipse with a rational center (x_num/x_den, y_num/y_den), a rational width w_num/w_den and a rational height h_num/h_den is generated" << std::endl;
    std::cerr << "  with an optional rigid motion composing a rational rotation defined by " << std::endl;
    std::cerr << "  a Pythagorian triple Ra, Rb, Rc s.t. sin = Ra/Rc and cos = Rb/Rc " << std::endl;
    std::cerr << "  and a rational translation defined by two rational numbers (=four integers) Tx_n/Tx_d and Ty_n/ty_d." << std::endl;
    std::cerr << "Example: " << argv[0] << "0 1 0 1 10 1 3 1 0 1 1 1 2 2 3 output.pgm " << std::endl;
}

int main(int argc, char **argv) {

    int shape_par_num = 8;
    int motion_par_num = 7;

    if (argc != (shape_par_num+2) && argc != (shape_par_num+motion_par_num+2) ) {
        usage(argc, argv);
        return 1;
    }

    bool motion = false;
    if (argc == (shape_par_num+motion_par_num+2))
        motion = true;

    typedef HyperRectDomain <Z2> Domain;
    typedef Domain::ConstIterator DomainConstIterator;
    typedef ImageContainerBySTLVector<Z2i::Domain, unsigned char> Image2D;

    // read/make a rational disk
    mpq_class center_x (atoi(argv[1]), atoi(argv[2]));
    mpq_class center_y (atoi(argv[3]), atoi(argv[4]));
    mpq_class a (atoi(argv[5]), atoi(argv[6]));
    mpq_class b (atoi(argv[7]), atoi(argv[8]));

    RationalEllipse rationalEllipse;
    rationalEllipse.set(center_x, center_y, a, b);
    cout << "Rational ellipse center: (" << center_x << ", " << center_y << ")" << endl;
    cout << "Rational ellipse width: 2*" << a << endl;
    cout << "Rational ellipse height: 2*" << b << endl;

    // set the rigid motion
    int Ra = 0;         // rational rotation with angle theta s.t. sin(theta) = Ra/Rc and cos(theta) = Rb/Rc
    int Rb = 1;         // initialize them as theata=0
    int Rc = 1;
    int Tx_num = 0;     // rational translation (Tx, Ty) s.t. Tx = Tx_num/Tx_den and Ty = Ty_num/Ty_den
    int Tx_den = 1;     // initialize it as (0/1,0/1)
    int Ty_num = 0;
    int Ty_den = 1;
    if (motion) {
        Ra = atoi(argv[shape_par_num+1]);
        Rb = atoi(argv[shape_par_num+2]);
        Rc = atoi(argv[shape_par_num+3]);
        Tx_num = atoi(argv[shape_par_num+4]);
        Tx_den = atoi(argv[shape_par_num+5]);
        Ty_num = atoi(argv[shape_par_num+6]);
        Ty_den = atoi(argv[shape_par_num+7]);
    }
    mpq_class cs(Rb, Rc);
    mpq_class sn(Ra, Rc);
    mpq_class tx(Tx_num, Tx_den);
    mpq_class ty(Ty_num, Ty_den);

    cout << "Rotation angle cos = " << cs << endl;
    cout << "Rotation angle sin = " << sn << endl;
    cout << "Translation x = " << tx << endl;
    cout << "Translation y = " << ty << endl;

    // set the image domain
    Point lowerPoint(rationalEllipse.getLowerBound());
    Point upperPoint(rationalEllipse.getUpperBound());
    if ( motion ) {
        if ( tx.get_d() < 0 )
            lowerPoint += Point((int)tx.get_d(),0);
        else
            upperPoint += Point((int)tx.get_d(),0);

        if (ty.get_d() < 0 )
            lowerPoint += Point(0, (int)ty.get_d());
        else
            upperPoint += Point(0, (int)ty.get_d());
    }
    Domain myDomain(lowerPoint, upperPoint);   // set the domain
    cout << "Image domain: " << lowerPoint << " - " << upperPoint << endl;

    // read/make an implicite function
    /*
    string s = string(argv[1]);
    std::istringstream sin(s);
    MPolynomial<2, int> P; // bivariate and integer coeffiecients
    sin >> P;
    trace.info() << "Read the bivariate polynomial: P(X_0, X_1) = " << P << std::endl;
    */

    // Gauss digitization without rigid motion
    Image2D image = Image2D(myDomain);
    /*
    for (auto point : image.domain()) {

        //if ( P.operator()(point[0])(point[1]) <= 0 )       // verify the function value at each pixel point:
            image.setValue(point, 255);                 // if non positive, the point is inside the object;
        else
            image.setValue(point, 0);                   // if positive, the point is outside.
    }
    */

    // Gauss digitization with rigid motion
    Image2D image_moved = Image2D( myDomain );

    for (auto point : image_moved.domain() ) {

        // new pixel point coordinates
        mpq_class px(point[0], 1);
        mpq_class py(point[1], 1);
        //cout<< "(px, py) = " << px << ", " << py << endl;

        // the value of this pixel is calculated without the rigid motion
        if (rationalEllipse.isInside(px, py))
            image.setValue(point, 255);
        else
            image.setValue(point, 0);

        // apply the inverse rigid motion to each new pixel point
        if (motion) {
            mpq_class qx = cs * (px - tx) + sn * (py - ty);
            mpq_class qy = -sn * (px - tx) + cs * (py - ty);
            //cout << "(qx, qy) = " << qx << ", " << qy << endl;

            // the value of the pixel after the rigid motion is calculated
            if (rationalEllipse.isInside(qx, qy) )
                image_moved.setValue(point, 255);
            else
                image_moved.setValue(point, 0);
        }
    }

    //cout << P.operator()(0.1)(0.2) << endl;
    //cout << P.operator()(0)(0) << endl;

    // generate the output file
    string filename = string(argv[argc-1]);
    image >> filename;

    if ( motion ) {
        size_t pos = filename.find_last_of(".");
        string filename2 = filename.substr(0,pos);
        filename2.append("_moved.pgm");
        image_moved >> filename2;
    }

    return 0;
}
