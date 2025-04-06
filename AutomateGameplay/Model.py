import torch
import torch.nn as nn
from torch.utils.data import Dataset
from torch.utils.data import DataLoader

import Constants


device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')


class PiecesDataset(Dataset):
    def __init__(self, images, labels, num_repeats=Constants.NUM_REPEATS_DATASET):
        self.images = torch.tensor(images).float()
        self.labels = torch.tensor(labels)
        self.num_repeats = num_repeats

        self.images = self.images.repeat(num_repeats, 1, 1, 1)
        self.labels = self.labels.repeat(num_repeats)

        self.images = self.images.permute(0, 3, 1, 2)

    def __len__(self):
        return len(self.images)

    def __getitem__(self, idx):
        return self.images[idx], self.labels[idx]


dataset = None
dataloader = None


class ConvolutionalNeuralNetwork(nn.Module):
    def __init__(self):
        super(ConvolutionalNeuralNetwork, self).__init__()

        self.model = nn.Sequential(
            nn.Conv2d(Constants.NUM_COLOR_CHANNELS, 16, kernel_size=3, stride=1, padding=1),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=2, stride=2),

            nn.Conv2d(16, 32, kernel_size=3, stride=1, padding=1),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=2, stride=2),

            nn.Conv2d(32, 64, kernel_size=3, stride=1, padding=1),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=2, stride=2),

            nn.Flatten(),
            nn.Linear(64 * (Constants.TEMPLATE_HEIGHT // 8) * (Constants.TEMPLATE_WIDTH // 8), 32),
            nn.ReLU(),
            nn.Linear(32, Constants.NUM_CLASSES)
        )

    def forward(self, X):
        X = self.model(X)
        return X


model = ConvolutionalNeuralNetwork()


def build_dataset_and_dataloader(X, y, batch_size=Constants.BATCH_SIZE):
    global dataset
    global dataloader

    dataset = PiecesDataset(X, y)
    dataloader = DataLoader(dataset, batch_size=batch_size, shuffle=True)


def train_model(num_epochs=Constants.NUM_EPOCHS, learning_rate=Constants.LEARNING_RATE):
    global model
    global dataloader
    global device

    loss_function = nn.CrossEntropyLoss(weight=torch.tensor(Constants.CLASS_WEIGHTS).float().to(device))
    optimizer = torch.optim.Adam(model.parameters(), lr=learning_rate)

    for epoch in range(num_epochs):
        model.train()

        train_current_epoch_loss = 0.0
        train_current_epoch_correct_predictions = 0
        train_current_epoch_total_predictions = 0

        for batch_idx, (X_batch, y_batch) in enumerate(dataloader):
            X_batch = X_batch.to(device).float()
            y_batch = y_batch.to(device)

            optimizer.zero_grad()
            outputs = model(X_batch)
            loss = loss_function(outputs, y_batch)
            loss.backward()
            optimizer.step()

            train_current_epoch_loss += loss.item()
            train_current_epoch_correct_predictions += (outputs.argmax(dim=1) == y_batch).sum().item()
            train_current_epoch_total_predictions += y_batch.size(0)

        print(f'Training Epoch {epoch + 1}/{num_epochs}, Loss: {train_current_epoch_loss}, Accuracy: {train_current_epoch_correct_predictions / train_current_epoch_total_predictions}')


def inference(X):
    global model
    global device

    X = torch.tensor(X).float()
    X = X.permute(2, 0, 1)
    X = X.unsqueeze(dim=0)

    model.eval()

    with torch.no_grad():
        X = X.to(device)
        outputs = model(X)
        predicted = outputs.argmax(dim=1)

    return Constants.FROM_LABEL_TO_PIECE[predicted.item()]



