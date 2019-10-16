#include <sstream>
#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
#include "Image.h"
#include "GameLib/Math.h"
#include "Vector2.h"
#include "Matrix22.h"
using namespace GameLib;
using namespace std;

int round( double a ){
	a += ( a > 0.0 ) ? 0.5 : -0.5f;
	return static_cast< int >( a );
}

void rotate(
int* rx,
int* ry,
int x,
int y,
const Vector2& offset,
const Matrix22& matrix ){
	Vector2 p( x, y );
	//�I�t�Z�b�g��0.5���܂Ƃ߂�
	Vector2 tmpOffset( -0.5, -0.5 );
	tmpOffset += offset; //tmpOffset = offset-(0.5,0.5)
	//�Y����������W�ɕϊ����āA���_�����炷
	p -= tmpOffset;
	//�s��������Ă܂킷
	matrix.multiply( &p, p );
	//���_�����ɖ߂��ēY������
	p += tmpOffset;
	//�l�̌ܓ����Đ�����
	*rx = round( p.x );
	*ry = round( p.y );
}

bool gFirstFrame = true;
Image* gImage;
int gCount;

namespace GameLib{
	void Framework::update(){
		if ( gFirstFrame ){
			gImage = new Image( "background.dds" );
			gFirstFrame = false;
		}
		unsigned* vram = videoMemory();
		int ww = width(); //window width
		int wh = height(); //window height
		//��U�S���^������
		for ( int i = 0; i < ww * wh; ++i ){
			vram[ i ] = 0;
		}
		int iw = gImage->width(); //image width
		int ih = gImage->height(); //image height
		Vector2 offset;
		offset.x = static_cast< double >( iw ) / 2.0;
		offset.y = static_cast< double >( ih ) / 2.0;
		double rotation = static_cast< double >( gCount );
		double sine = sin( rotation );
		double cosine = cos( rotation );
		Matrix22 matrix( cosine, -sine, sine, cosine );
		for ( int y = 0; y < ih; ++y ){
			for ( int x = 0; x < iw; ++x ){
				//��]����v�Z
				int rx, ry;
				rotate( &rx, &ry, x, y, offset, matrix );
				//�͈͓��Ȃ�\��t��
				if ( rx >= 0 && rx < ww && ry >= 0 && ry < wh ){
					vram[ ry * ww + rx ] = gImage->pixel( x, y );
				}
			}
		}
		++gCount;

		ostringstream oss;
		oss << frameRate();
		drawDebugString( 0, 0, oss.str().c_str() );
	}
}