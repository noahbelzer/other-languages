# Use the virtual environment: source deep_learning_1/bin/activate

import torch
import torch.nn as nn
import torch.optim as optim
import numpy as np

# --- Real data distribution ---
# We'll try to learn N(4, 1.5) — a Gaussian centered at 4
def real_data_sampler(n):
    return torch.FloatTensor(np.random.normal(4.0, 1.5, (n, 1)))

# --- Noise input to Generator ---
def noise_sampler(n, z_dim):
    return torch.FloatTensor(np.random.normal(0, 1, (n, z_dim)))

# --- Generator ---
# Takes z_dim noise -> tries to produce data that looks like real_data
class Generator(nn.Module):
    def __init__(self, z_dim):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(z_dim, 16),
            nn.ReLU(),
            nn.Linear(16, 1)
        )

    def forward(self, z):
        return self.net(z)

# --- Discriminator ---
# Takes 1D data -> outputs probability it's real (1) vs fake (0)
class Discriminator(nn.Module):
    def __init__(self):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(1, 16),
            nn.ReLU(),
            nn.Linear(16, 1),
            nn.Sigmoid()
        )

    def forward(self, x):
        return self.net(x)

# --- Setup ---
z_dim = 4
batch_size = 64
epochs = 5000
lr = 0.001

G = Generator(z_dim)
D = Discriminator()

opt_G = optim.Adam(G.parameters(), lr=lr)
opt_D = optim.Adam(D.parameters(), lr=lr)

loss_fn = nn.BCELoss()  # Binary cross-entropy

# --- Training loop ---
for epoch in range(epochs):

    # === Train Discriminator ===
    real = real_data_sampler(batch_size)
    fake = G(noise_sampler(batch_size, z_dim)).detach()  # detach: don't backprop into G yet

    real_labels = torch.ones(batch_size, 1)
    fake_labels = torch.zeros(batch_size, 1)

    loss_D = loss_fn(D(real), real_labels) + loss_fn(D(fake), fake_labels)

    opt_D.zero_grad()
    loss_D.backward()
    opt_D.step()

    # === Train Generator ===
    z = noise_sampler(batch_size, z_dim)
    fake = G(z)

    # Generator wants D to think its output is real
    loss_G = loss_fn(D(fake), real_labels)

    opt_G.zero_grad()
    loss_G.backward()
    opt_G.step()

    if epoch % 500 == 0:
        print(f"Epoch {epoch:5d} | D loss: {loss_D.item():.4f} | G loss: {loss_G.item():.4f}")

# --- Check results ---
with torch.no_grad():
    samples = G(noise_sampler(1000, z_dim)).numpy()
    print(f"\nGenerated mean: {samples.mean():.3f}  (target: 4.0)")
    print(f"Generated std:  {samples.std():.3f}  (target: 1.5)")