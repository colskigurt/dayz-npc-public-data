# 최신 실험 요약

64개체 정지 Run: 8파트 대 5파트, 독립 비교 1쌍(각15초). Present p95 30.75→31.28ms로 개선 없음. CPU Busy p95 15.56→13.69ms. 실제 SyncInterval=1, VSync OFF 미확인. 진단 성공·최적화 수용 보류. AI/추적/커스텀 접촉·기능 회귀는 이번 비교에서 미실행. 이전 헤드리스 3쌍 역시 효과 불일치. 20ms 목표 미달성.

[상세 보고서](operations/npc-palette64-client-present-first-pair-20261105.md)


## 후속 정적 진단 — 새 실행 없음

루트 요청 VSync=0과 실행 후 하위 사용자 설정 VSync=1 불일치를 두 조건에서 확인했다. 다른 그래픽 옵션도 요청과 달랐다. 다음 격리 시험 생성 도구의 저장 경로를 보완했지만 실제 적용과 성능 효과는 미검증이다.

[설정 경로 진단](operations/npc-palette64-client-config-path-20261105.md)
