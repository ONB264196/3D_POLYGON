#define _CYLINDER

#ifdef _3D_COODINATE
#include"libOne.h"
void gmain() {
    window(800, 800);
    VECTOR p[3];
    p[0].set(-1, 1, 0);
    p[1].set(-1, -1, 0);
    p[2].set(1, -1, 0);

    while (notQuit) {
        clear(60);
        triangle3D(p[0], p[1], p[2]);
    }
}
#endif

#ifdef _3D_BORD
#include"libOne.h"
void gmain() {
    window(1920, 1080, 1);
    const int numVertices = 4;
    VECTOR op[numVertices]{};
    op[0].set(0, 0, -1);
    op[1].set(0, 0, 0);
    op[2].set(1, 0, 0);
    op[3].set(1, 0, -1);
    VECTOR p[numVertices]{};

    MATRIX world{}, view{}, proj{};
    float angleX = 0.5f;
    float angleY = 0;
    float radius = 8;

    float cols = 50;
    float rows = 50;
    float theta = 0;
    int sw = 1;

    COLOR c[2]{};
    c[0].set(0, 0, 0);
    c[1].set(31, 30, 51);
    while (notQuit) {
        clear(225, 226, 205);

        //view
        if (isPress(KEY_RIGHT)) angleY += 0.0085f;
        if (isPress(KEY_LEFT)) angleY -= 0.0085f;
        if (isPress(KEY_UP)) angleX += 0.0085f;
        if (isPress(KEY_DOWN)) angleX -= 0.0085f;
        VECTOR camPos, lookat(0, 0, 0), up(0, 1, 0);
        camPos.x = sin(angleY) * cos(angleX) * radius;
        camPos.y =               sin(angleX) * radius;
        camPos.z = cos(angleY) * cos(angleX) * radius;
        view.camera(camPos, lookat, up);

        //proj
        float fov = 3.14f / 2;
        float aspect = width / height;
        float _near = 1;
        float _far = 100;
        proj.pers(fov, aspect, _near, _far);

        if (isTrigger(KEY_SPACE)) sw *= -1;
        if (sw == -1) {
            theta += 0.005f;
        }

        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                world.identity();
                world.mulTranslate(-cols / 2 + col, (row * rows + col) * 0.000001f , rows / 2 + -row);
                world.mulRotateY(theta);
                for (int i = 0; i < numVertices; i++) {
                    //World���W�ϊ�
                    p[i] = world * op[i];
                    //view���W�ϊ�
                    p[i] = view * p[i];
                    //projection���W�ϊ�
                    p[i] = proj * p[i];
                }
                int n = (row + col) % 2;
                triangle3D(p[0], p[1], p[2], c[n], c[n], c[n]);
                triangle3D(p[0], p[2], p[3], c[n], c[n], c[n]);
            }
        }
    }

}
#endif

#ifdef _LIGHTSIMULATION
#include"libOne.h"
void gmain() {
    window(1920, 1080, 1);
    //���_���W
    const int numVertices = 4;
    VECTOR op[numVertices]{};
    op[0].set(-0.5f, 0.5f, 0);
    op[1].set(-0.5f, -0.5f, 0);
    op[2].set(0.5f, 0.5f, 0);
    op[3].set(0.5f, -0.5f, 0);
    VECTOR p[numVertices]{};
    //�s��
    MATRIX world{}, view{}, proj{};
    view.camera(VECTOR(0, 0, 2), VECTOR(0, 0, 0), VECTOR(0, 1, 0));
    proj.pers(45, width / height, 1, 10);
    //��]�p
    int sw = 1;
    float thetaX = 0;
    float thetaY = 0;
    //�ړ��p
    float posX = 0;
    float posY = 0;
    //���C�����[�v
    while (notQuit) {
        clear(225, 224, 205);
        if (isTrigger(KEY_SPACE)) sw *= -1;
        if (sw == 1) thetaY += 0.0085f;
        else {
            if (isPress(KEY_A)) posX -= 0.005f;
            if (isPress(KEY_D)) posX += 0.005f;
            if (isPress(KEY_W)) posY += 0.005f / (width / height);
            if (isPress(KEY_S)) posY -= 0.005f / (width / height);
            if (isPress(KEY_UP)) thetaX -= 0.0085f;
            if (isPress(KEY_DOWN)) thetaX += 0.0085f;
            if (isPress(KEY_LEFT)) thetaY -= 0.0085f;
            if (isPress(KEY_RIGHT)) thetaY += 0.0085f;
            if (isTrigger(KEY_R)) posX = posY = thetaX = thetaY = 0;
        }
        //world�ϊ��s������
        world.identity();
        world.mulTranslate(posX, posY, 0);
        world.mulRotateX(thetaX);
        world.mulRotateY(thetaY);
        world.mulScaling(0.5f, 0.5f, 0.5f);
        //�s��ō��W�ϊ�
        for (int i = 0; i < numVertices; i++) {
            p[i] = world * op[i];
            p[i] = view * p[i];
            p[i] = proj * p[i];
        }
        //�B�ʏ���
        VECTOR a = p[1] - p[0];
        VECTOR b = p[2] - p[0];
        if (a.crossZ(b) < 0) continue;

        //�F�Ɩ��邳���v�Z
        COLOR oColor(52, 81, 164);
        VECTOR light(0, 0, 1);
        VECTOR oNormal(0, 0, 1);
        MATRIX mat = world;
        mat._14 = mat._24 = mat._34 = 0;
        VECTOR normal = normalize(mat * oNormal);
        float bright = max(0, dot(light, normal));
        COLOR c = oColor * bright;

        //�`��
        triangle3D(p[0], p[1], p[2], c, c, c);
        triangle3D(p[2], p[1], p[3], c, c, c);
    }


}
#endif

#ifdef _CYLINDER
#include"libOne.h"
void gmain() {
    //�E�B���h�E�\���A�����ݒ�
    window(1920, 1080, full);
    hideCursor();
    angleMode(DEGREES);

    //���_�̍��W
    const int numRect = 16;
    const int numVertices = 4 * numRect;
    VECTOR op[numVertices];
    float s, c, angle = 360.0f / numRect;
    float radius = 0.5f;
    float high = 0.5f;
    float low = -0.5f;
    for (int i = 0; i < numRect; i++) {
        //�~����ɂS�p�`�̒��_��z�u
        int j = i * 4;
        s = sin(angle * i) * radius;
        c = cos(angle * i) * radius;
        op[j + 0].set(s, high, c);//����
        op[j + 1].set(s, low, c);//����
        s = sin(angle * (i + 1)) * radius;
        c = cos(angle * (i + 1)) * radius;
        op[j + 2].set(s, high, c);//�E��
        op[j + 3].set(s, low, c);//�E��
    }
    //���W�ϊ���̒��_
    VECTOR p[numVertices];

    //���_���Ƃɖ@���x�N�g������������i���[�v�̒��Œl��ݒ�j
    VECTOR on[numVertices];
    int sw = 1;//���ʁA�^���Ȗʂ̐؂�ւ��X�C�b�`
    int firstFlag = 1;//���[�v����͕K���@����ݒ�

    //���_���ƂɐF���v�Z�������Ɋi�[
    COLOR col[numVertices];

    //�s��
    MATRIX world{}, view{}, proj{};
    view.camera(VECTOR(0, 1, 3), VECTOR(0, 0, 0), VECTOR(0, 1, 0));
    proj.pers(45, width / height, 1, 100);

    //���C�g
    VECTOR light;
    float theta = 10;
    int rotateFlag = 0;

    float thetaX = 0;
    float thetaY = 0;
    float posX = 0;
    float posY = 0;

    //���C�����[�v
    while (notQuit) {
        if (isPress(KEY_A)) posX -= 0.005f;
        if (isPress(KEY_D)) posX += 0.005f;
        if (isPress(KEY_W)) posY += 0.005f / (width / height);
        if (isPress(KEY_S)) posY -= 0.005f / (width / height);
        if (isPress(KEY_UP)) thetaX -= 0.1f;
        if (isPress(KEY_DOWN)) thetaX += 0.1f;
        if (isPress(KEY_LEFT)) thetaY -= 0.1f;
        if (isPress(KEY_RIGHT)) thetaY += 0.1f;
        if (isTrigger(KEY_R)) posX = posY = thetaX = thetaY = 0;

        //�@���ݒ�؂�ւ�
        if (isTrigger(KEY_Z) || firstFlag) {
            firstFlag = 0;
            ++sw %= 2;
            if (sw == 0) {
                //���ʂƂ��Ă̖@���x�N�g��
                VECTOR a, b, n;
                for (int i = 0; i < numRect; i++) {
                    int j = i * 4;
                    a = op[j + 1] - op[j];
                    b = op[j + 2] - op[j];
                    n = normalize(cross(a, b));
                    on[j + 0] = n;
                    on[j + 1] = n;
                    on[j + 2] = n;
                    on[j + 3] = n;
                }
            }
            else {
                //�ȖʂɌ�����悤�ɂ���@���x�N�g��
                for (int i = 0; i < numRect; i++) {
                    int j = i * 4;
                    s = sin(angle * i);
                    c = cos(angle * i);
                    on[j + 0].set(s, 0, c);
                    on[j + 1].set(s, 0, c);
                    s = sin(angle * (i + 1));
                    c = cos(angle * (i + 1));
                    on[j + 2].set(s, 0, c);
                    on[j + 3].set(s, 0, c);
                }
            }
        }

        //���C�g��]����
        if (isTrigger(KEY_X)) {
            rotateFlag = ~rotateFlag;
        }
        if (rotateFlag) {
            theta += 1;
        }
        light.set(sin(theta), 0, cos(theta));

        //world�ϊ��s�������
        world.identity();
        world.mulTranslate(posX, posY, 0);
        world.mulRotateX(thetaX);
        world.mulRotateY(thetaY);
        //�P���_���ƂɁu���W�ϊ��v�Ɓu�F�̌v�Z�v���s��
        for (int i = 0; i < numVertices; i++) {
            //���W�ϊ�
            p[i] = world * op[i];
            p[i] = view * p[i];
            p[i] = proj * p[i];
            //�F�Ɩ��邳�̌v�Z
            COLOR oColor(52, 81, 164);
            MATRIX worldRS = world;//�ړ��v�f0�̍s��ɂ���Ɖ�]�g��}�g���b�N�X�ɂȂ�
            worldRS._14 = worldRS._24 = worldRS._34 = 0;
            VECTOR normal = normalize(worldRS * on[i]);
            float bright = max(0.3f, dot(light, normal));
            col[i] = oColor * bright;
        }

        //�S�p�`��numRect���`��
        clear(225, 224, 205);
        for (int i = 0; i < numRect; i++) {
            int j = i * 4;
            triangle3D(p[j + 0], p[j + 1], p[j + 2], col[j + 0], col[j + 1], col[j + 2]);
            triangle3D(p[j + 2], p[j + 1], p[j + 3], col[j + 2], col[j + 1], col[j + 3]);
        }
    }
}
#endif
