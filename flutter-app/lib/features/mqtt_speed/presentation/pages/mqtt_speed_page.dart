import 'package:flutter/material.dart';
import 'package:flutter_hooks/flutter_hooks.dart';
import 'package:hooks_riverpod/hooks_riverpod.dart';

class MqttSpeedPage extends HookWidget {
  const MqttSpeedPage({super.key});

  @override
  Widget build(BuildContext context) {
    final isConnected = useState(false);
    final currentSpeed = useState(0.0);
    final notificationsEnabled = useState(true);
    final speedHistory = useState<List<double>>([]);

    return Scaffold(
      backgroundColor: const Color(0xFFF8F9FA),
      appBar: AppBar(
        backgroundColor: Colors.white,
        elevation: 0,
        title: const Text(
          'Vehicle Speed Monitor',
          style: TextStyle(fontWeight: FontWeight.w600),
        ),
        actions: [
          Switch(
            value: notificationsEnabled.value,
            onChanged: (value) => notificationsEnabled.value = value,
          ),
          const SizedBox(width: 16),
        ],
      ),
      body: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          children: [
            Container(
              padding: const EdgeInsets.all(20),
              decoration: BoxDecoration(
                color: Colors.white,
                borderRadius: BorderRadius.circular(16),
                boxShadow: [
                  BoxShadow(
                    color: Colors.black.withOpacity(0.05),
                    blurRadius: 10,
                    offset: const Offset(0, 2),
                  ),
                ],
              ),
              child: Column(
                children: [
                  Row(
                    mainAxisAlignment: MainAxisAlignment.spaceBetween,
                    children: [
                      const Text(
                        'Connection Status',
                        style: TextStyle(
                          fontSize: 18,
                          fontWeight: FontWeight.w600,
                        ),
                      ),
                      Container(
                        padding: const EdgeInsets.symmetric(
                          horizontal: 12,
                          vertical: 6,
                        ),
                        decoration: BoxDecoration(
                          color: isConnected.value ? Colors.green : Colors.red,
                          borderRadius: BorderRadius.circular(20),
                        ),
                        child: Text(
                          isConnected.value ? 'Connected' : 'Disconnected',
                          style: const TextStyle(
                            color: Colors.white,
                            fontSize: 12,
                            fontWeight: FontWeight.w500,
                          ),
                        ),
                      ),
                    ],
                  ),
                  const SizedBox(height: 20),
                  Row(
                    mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                    children: [
                      ElevatedButton.icon(
                        onPressed: () => isConnected.value = !isConnected.value,
                        icon: Icon(
                          isConnected.value ? Icons.stop : Icons.play_arrow,
                        ),
                        label: Text(
                          isConnected.value ? 'Disconnect' : 'Connect',
                        ),
                        style: ElevatedButton.styleFrom(
                          backgroundColor: isConnected.value
                              ? Colors.red
                              : Theme.of(context).colorScheme.primary,
                          foregroundColor: Colors.white,
                        ),
                      ),
                      ElevatedButton.icon(
                        onPressed: () {
                          if (isConnected.value) {
                            currentSpeed.value = (currentSpeed.value + 5) % 120;
                            speedHistory.value = [
                              ...speedHistory.value,
                              currentSpeed.value,
                            ];
                            if (speedHistory.value.length > 10) {
                              speedHistory.value =
                                  speedHistory.value.sublist(1);
                            }
                          }
                        },
                        icon: const Icon(Icons.refresh),
                        label: const Text('Test Speed'),
                      ),
                    ],
                  ),
                ],
              ),
            ),
            const SizedBox(height: 24),
            Container(
              padding: const EdgeInsets.all(20),
              decoration: BoxDecoration(
                color: Colors.white,
                borderRadius: BorderRadius.circular(16),
                boxShadow: [
                  BoxShadow(
                    color: Colors.black.withOpacity(0.05),
                    blurRadius: 10,
                    offset: const Offset(0, 2),
                  ),
                ],
              ),
              child: Column(
                children: [
                  const Text(
                    'Current Speed',
                    style: TextStyle(
                      fontSize: 16,
                      fontWeight: FontWeight.w500,
                      color: Colors.grey,
                    ),
                  ),
                  const SizedBox(height: 8),
                  Text(
                    '${currentSpeed.value.toStringAsFixed(1)} km/h',
                    style: TextStyle(
                      fontSize: 48,
                      fontWeight: FontWeight.bold,
                      color: currentSpeed.value > 80
                          ? Colors.red
                          : currentSpeed.value > 60
                              ? Colors.orange
                              : Colors.green,
                    ),
                  ),
                  const SizedBox(height: 16),
                  LinearProgressIndicator(
                    value: currentSpeed.value / 120,
                    backgroundColor: Colors.grey[200],
                    valueColor: AlwaysStoppedAnimation<Color>(
                      currentSpeed.value > 80
                          ? Colors.red
                          : currentSpeed.value > 60
                              ? Colors.orange
                              : Colors.green,
                    ),
                  ),
                ],
              ),
            ),
            const SizedBox(height: 24),
            Expanded(
              child: Container(
                padding: const EdgeInsets.all(20),
                decoration: BoxDecoration(
                  color: Colors.white,
                  borderRadius: BorderRadius.circular(16),
                  boxShadow: [
                    BoxShadow(
                      color: Colors.black.withOpacity(0.05),
                      blurRadius: 10,
                      offset: const Offset(0, 2),
                    ),
                  ],
                ),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    const Text(
                      'Speed History',
                      style: TextStyle(
                        fontSize: 18,
                        fontWeight: FontWeight.w600,
                      ),
                    ),
                    const SizedBox(height: 16),
                    Expanded(
                      child: speedHistory.value.isEmpty
                          ? const Center(
                              child: Text(
                                'No speed data yet',
                                style: TextStyle(
                                  color: Colors.grey,
                                  fontSize: 16,
                                ),
                              ),
                            )
                          : ListView.builder(
                              itemCount: speedHistory.value.length,
                              itemBuilder: (context, index) {
                                final speed = speedHistory.value[index];
                                return ListTile(
                                  leading: Icon(
                                    Icons.speed,
                                    color: speed > 80
                                        ? Colors.red
                                        : speed > 60
                                            ? Colors.orange
                                            : Colors.green,
                                  ),
                                  title: Text(
                                    '${speed.toStringAsFixed(1)} km/h',
                                    style: const TextStyle(
                                      fontWeight: FontWeight.w500,
                                    ),
                                  ),
                                  subtitle: Text(
                                    'Recorded ${speedHistory.value.length - index} readings ago',
                                  ),
                                  trailing: Text(
                                    _getSpeedStatus(speed),
                                    style: TextStyle(
                                      color: speed > 80
                                          ? Colors.red
                                          : speed > 60
                                              ? Colors.orange
                                              : Colors.green,
                                      fontWeight: FontWeight.w500,
                                    ),
                                  ),
                                );
                              },
                            ),
                    ),
                  ],
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }

  String _getSpeedStatus(double speed) {
    if (speed > 80) return 'High';
    if (speed > 60) return 'Medium';
    return 'Normal';
  }
}
