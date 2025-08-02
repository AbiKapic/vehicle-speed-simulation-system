import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:flutter_local_notifications/flutter_local_notifications.dart';
import 'core/app.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();

  final flutterLocalNotificationsPlugin = FlutterLocalNotificationsPlugin();
  const androidSettings = AndroidInitializationSettings('@mipmap/ic_launcher');
  const iosSettings = DarwinInitializationSettings();
  const initSettings =
      InitializationSettings(android: androidSettings, iOS: iosSettings);
  await flutterLocalNotificationsPlugin.initialize(initSettings);

  runApp(const ProviderScope(child: VehicleSpeedMonitorApp()));
}
