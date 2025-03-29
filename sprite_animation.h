struct Obstacle {
    int x, width, gap;
    int topHeight, bottomHeight;
    int speed = 5;
    int direction = 1;
    int moveSpeed = 2;

    bool isMoving = false;
    int moveTimer = 0;  // Đếm thời gian trước khi bắt đầu zigzag

    SDL_Texture* topTexture = nullptr;
    SDL_Texture* bottomTexture = nullptr;

    void init(Graphics& graphics, const char* topFile, const char* bottomFile, int startX) {
        topTexture = graphics.loadTexture(topFile);
        bottomTexture = graphics.loadTexture(bottomFile);
        x = startX;
        width = 100;
        gap = 250;
        resetPosition();
    }

    void resetPosition() {
        topHeight = 100 + rand() % 200;
        bottomHeight = SCREEN_HEIGHT - (topHeight + gap);
        isMoving = false;  // Khi reset, ống sẽ đứng yên
        moveTimer = 0;
    }

    void update() {
        x -= speed;

        if (moveTimer > 0) {
            moveTimer--;
        } else {
            isMoving = true;
        }

        if (isMoving) {
            topHeight += direction * moveSpeed;
            bottomHeight = SCREEN_HEIGHT - (topHeight + gap);

            if (topHeight <= 50 || topHeight >= SCREEN_HEIGHT - gap - 50) {
                direction *= -1;
                isMoving = false;  // Khi đổi hướng, tạm thời dừng zigzag
                moveTimer = rand() % 200 + 50;  // Chờ ngẫu nhiên 50-250 frame rồi mới zigzag tiếp
            }
        }

        if (x < -width) {
            x = SCREEN_WIDTH;
            resetPosition();
        }
    }

    void render(Graphics& graphics) {
        SDL_Rect topRect = { x, 0, width, topHeight };
        SDL_Rect bottomRect = { x, SCREEN_HEIGHT - bottomHeight, width, bottomHeight };

        SDL_RenderCopy(graphics.renderer, topTexture, NULL, &topRect);
        SDL_RenderCopy(graphics.renderer, bottomTexture, NULL, &bottomRect);
    }

    bool checkCollision(const Mouse& mouse) {
        return (mouse.x + 80 > x && mouse.x < x + width &&
                (mouse.y < topHeight || mouse.y + 80 > SCREEN_HEIGHT - bottomHeight));
    }
};
