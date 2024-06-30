let playerImg, boxImg, backgroundImg, obstacleImg, bgSound;
let player;
let boxes = [];
let targets = [];
let gridSize = 50;
let currentLevel = 0;
let focusedOnSelect = false;
let gameWidth, gameHeight, offsetX, offsetY;
let levelSelect;

let levels = [
  [
    [1, 1, 1, 1, 1, 1, 1, 1],
    [1, 4, 0, 0, 0, 1, 0, 1],
    [1, 0, 1, 1, 0, 1, 0, 1],
    [1, 2, 1, 3, 2, 0, 0, 1],
    [1, 0, 1, 0, 0, 0, 0, 1],
    [1, 3, 0, 0, 3, 2, 0, 1],
    [1, 1, 1, 1, 1, 1, 1, 1],
  ],
  [
    [1, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 4, 2, 0, 0, 0, 0, 3, 1],
    [1, 2, 1, 3, 0, 1, 2, 0, 1],
    [1, 0, 1, 2, 0, 3, 0, 0, 1],
    [1, 3, 0, 0, 0, 1, 0, 1, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1],
  ],
  [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 4, 1],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, 1, 2, 0, 1, 1],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, 0, 0, 2, 1, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 0, 2, 0, 1, 0],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2, 0, 1, 0, 1, 1, 1],
    [1, 3, 3, 3, 3, 0, 0, 0, 1, 1, 0, 2, 0, 0, 2, 0, 0, 1],
    [1, 1, 3, 3, 3, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 1],
    [1, 3, 3, 3, 3, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
   ],
   [	
   [0, 0, 1, 1, 1, 1, 1],
   [1, 1, 1, 0, 0, 0, 1],
   [1, 3, 4, 2, 0, 0, 1],
   [1, 1, 1, 0, 2, 3, 1],
   [1, 3, 1, 1, 2, 0, 1],
   [1, 0, 1, 0, 3, 0, 1, 1],
   [1, 2, 0, 2, 2, 2, 3, 1],
   [1, 0, 0, 0, 3, 0, 0, 1],
   [1, 1, 1, 1, 1, 1, 1, 1],
   ]
];

function preload() {
  playerImg = loadImage('player.png');
  boxImg = loadImage('box.png');
  backgroundImg = loadImage('b.png');
  obstacleImg = loadImage('stone.png');
  bgSound = loadSound('background.mp3'); 
}

function setup() {
  createCanvas(1280, 720).parent('game-container');
  gameWidth = levels[0][0].length * gridSize;
  gameHeight = levels[0].length * gridSize;
  offsetX = (width - gameWidth) / 2;
  offsetY = (height - gameHeight) / 2;
  loadLevel(currentLevel);

  levelSelect = createSelect();
  levelSelect.position(offsetX + 100, offsetY / 2 - 25);
  levelSelect.option('Level 1', 0);
  levelSelect.option('Level 2', 1);
  levelSelect.option('Level 3', 2);
  levelSelect.option('Level 4', 3);
  levelSelect.changed(function() {
    currentLevel = int(levelSelect.value());
    loadLevel(currentLevel);
    levelSelect.elt.blur(); 
    focusedOnSelect = false; 
  });

  levelSelect.mousePressed(() => focusedOnSelect = true);
  levelSelect.mouseReleased(() => focusedOnSelect = false);

  
  document.addEventListener('click', startGame, { once: true });
}

function startGame() {
  
  bgSound.loop();
}

function draw() {
  background(220);
  push();
  translate(offsetX, offsetY);
  drawGrid();
  drawPlayer();
  drawBoxes();
  drawTargets();
  pop();
  checkWin();

  fill(0);
  textSize(24);
  textAlign(CENTER, CENTER);
  text('Level ' + (currentLevel + 1), width / 2, offsetY / 2 - 25);
  text('Space to restart', width / 2 + 200, offsetY / 2 - 25);
}

function drawGrid() {
  let map = levels[currentLevel];
  for (let y = 0; y < map.length; y++) {
    for (let x = 0; x < map[y].length; x++) {
      if (map[y][x] === 1) {
        image(obstacleImg, x * gridSize, y * gridSize, gridSize, gridSize);
      } else {
        image(backgroundImg, x * gridSize, y * gridSize, gridSize, gridSize);
      }
    }
  }
}

function drawPlayer() {
  image(playerImg, player.x * gridSize, player.y * gridSize, gridSize, gridSize);
}

function drawBoxes() {
  for (let box of boxes) {
    image(boxImg, box.x * gridSize, box.y * gridSize, gridSize, gridSize);
  }
}

function drawTargets() {
  fill(0, 255, 0);
  for (let target of targets) {
    let isBoxOnTarget = boxes.some(box => box.equals(target));
    if (!isBoxOnTarget) {
      ellipse(target.x * gridSize + gridSize / 2, target.y * gridSize + gridSize / 2, gridSize / 2);
    }
  }
}

function keyPressed() {
  if (focusedOnSelect) return;

  let nextPos = player.copy();
  if (keyCode === UP_ARROW) {
    nextPos.y -= 1;
  } else if (keyCode === DOWN_ARROW) {
    nextPos.y += 1;
  } else if (keyCode === LEFT_ARROW) {
    nextPos.x -= 1;
  } else if (keyCode === RIGHT_ARROW) {
    nextPos.x += 1;
  } else if (keyCode === 32) {
    loadLevel(currentLevel);
    return;
  }

  if (isWalkable(nextPos)) {
    player = nextPos;
    startGame(); 
  }
}

function isWalkable(pos) {
  let map = levels[currentLevel];
  if (map[pos.y][pos.x] === 1) {
    return false;
  }
  for (let box of boxes) {
    if (box.equals(pos)) {
      let nextBoxPos = box.copy().add(p5.Vector.sub(pos, player));
      if (isInsideGrid(nextBoxPos) && !isBoxAt(nextBoxPos) && map[nextBoxPos.y][nextBoxPos.x] !== 1) {
        box.set(nextBoxPos);
        return true;
      } else {
        return false;
      }
    }
  }
  return isInsideGrid(pos);
}

function isBoxAt(pos) {
  for (let box of boxes) {
    if (box.equals(pos)) {
      return true;
    }
  }
  return false;
}

function isInsideGrid(pos) {
  let map = levels[currentLevel];
  return pos.x >= 0 && pos.x < map[0].length && pos.y >= 0 && pos.y < map.length;
}

function checkWin() {
  let win = boxes.every(box => {
    return targets.some(target => target.equals(box));
  });

  if (win) {
    fill(0);
    textSize(32);
    textAlign(CENTER, CENTER);
    text('You Win!', width / 2, height / 2);
    noLoop();
    setTimeout(() => {
      currentLevel = (currentLevel + 1) % levels.length;
      loadLevel(currentLevel);
      loop();
    }, 2000);
  }
}

function loadLevel(level) {
  let map = levels[level];
  boxes = [];
  targets = [];

  for (let y = 0; y < map.length; y++) {
    for (let x = 0; x < map[y].length; x++) {
      if (map[y][x] === 2) {
        boxes.push(createVector(x, y));
      } else if (map[y][x] === 3) {
        targets.push(createVector(x, y));
      } else if (map[y][x] === 4) {
        player = createVector(x, y);
      }
    }
  }

  gameWidth = map[0].length * gridSize;
  gameHeight = map.length * gridSize;
  offsetX = (width - gameWidth) / 2;
  offsetY = (height - gameHeight) / 2;

  if (levelSelect) {
    levelSelect.position(offsetX + 100, offsetY / 2 - 25);
  }

  loop();
}
