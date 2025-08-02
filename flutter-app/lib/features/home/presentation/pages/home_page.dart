import 'package:flutter/material.dart';
import 'package:flutter_hooks/flutter_hooks.dart';
import 'package:hooks_riverpod/hooks_riverpod.dart';
import '../../../chat/presentation/pages/chat_page.dart';
import '../../../mqtt_speed/presentation/pages/mqtt_speed_page.dart';

class HomePage extends HookWidget {
  const HomePage({super.key});

  @override
  Widget build(BuildContext context) {
    final currentIndex = useState(0);

    final pages = [
      const ChatPage(),
      const MqttSpeedPage(),
    ];

    return Scaffold(
      body: IndexedStack(
        index: currentIndex.value,
        children: pages,
      ),
      bottomNavigationBar: NavigationBar(
        selectedIndex: currentIndex.value,
        onDestinationSelected: (index) => currentIndex.value = index,
        destinations: const [
          NavigationDestination(
            icon: Icon(Icons.chat_bubble_outline),
            selectedIcon: Icon(Icons.chat_bubble),
            label: 'Chat',
          ),
          NavigationDestination(
            icon: Icon(Icons.speed_outlined),
            selectedIcon: Icon(Icons.speed),
            label: 'Speed Test',
          ),
        ],
      ),
    );
  }
}
