# DayZ NPC Observatory — 현재 상태

2026-11-09: **현재64본 모델의 골격 목록을248개로 확장한 격리 입력이 정적 검증과 단일 서버 생성을 통과했다.** 세LOD 형상·가중치·Memory·기준 위치 동일, 가중 본64개와 시각 sub-skeleton101개 유지. 명령 바인딩 및 detached 퇴역 확인, 신규PBO 서명 검증 성공.

- [최신 상세 보고서](reports/operations/cardinality64-factory-20261109.md)
- [구조화 결과](data/cardinality64-factory-20261109.json)
- [이전 기준 factory 실패 분리](reports/operations/cardinality-factory-crash-20261109.md)
- [본 매핑 해독 경계](reports/operations/odol-palette-mapping-20261109.md)
- [기존 보고서](reports/operations/)

**화면·전체 가중 본 통합·CPU 절감은 미검증.** 8/64 기능·반복 성능·20ms p95 목표는 미해결. 다음은 고정 네트워크 쌍의 CPU 포즈와 실제 화면 비교다. 기존 게임플레이·물리 코드는 변경하지 않았다.

33ms 밀침 묶음 처리는20Hz 콜백 근거로 보류하며 더 긴 지연을 임의 적용하지 않는다.
