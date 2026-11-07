# 이동 실행 비용 소유자와 복구 후보 탐색 절감 검토
작성: 2026-11-08

## 1. 작업 목표
이미 확보한 64개체 스크립트 프로파일과 현행 소스를 연결해 다음 국소 후보를 선정한다. 정상 밀침·낙하·착지·FOLLOW·재등반을 보존하며 기능 복구 개발로 범위를 확대하지 않는다.

## 2. 실제 수행
기존 ANIM-contact-owner-direct1의 named time/count 및 선택 함수 기록, 현행 Ground/Traversal/Agent 소스, 이전 support-entry-reuse1 결과를 읽었다. 새 게임 실행·후킹·PBO 생성·게임 코드 변경은 없다.
실제 실행 경로는 RunPrePhys→Body.Drive→Traversal.Drive/Follow→Walk→DriveGround→MoveTo 및 SolveContacts이고, 별도 Crowd.ConsumeContact→Displace→SolveContacts 경로가 있다. SurfaceWalk는 경로를 갱신한 후 Walk로 연결된다.

## 3. 측정 결과
기존 단일 진단은64개체·30.0001게임초·706프레임이다. DriveGround 28,147회, Displace 29,490회, ConsumeContact 36,790회, SolveContacts 70,626회, TrySupportBypass 412회. 이 값들은 새 실행 결과가 아니다.
같은 프로파일의 rawTime은 RunPrePhys 1,098,890, DriveGround 430,860, SolveContactsInternal 503,971, TrySupportBypass 99,696.6이다. 단위와 집계 설정이 완전히 검증되지 않았으며 중첩된다. 합산·차감해 CPU self-time이나 프레임 시간으로 쓰지 않는다. 최신 retail CPU 표본과 서로 다른 실행이므로 합치지 않는다.
기존 밀침 성공은28,452/29,490(96.48%). 실패/무변위만 막는 방식이 주력 해결책이라는 근거는 약하다. 이번 p50/p95/p99·시각·회귀는 미측정이다.

## 4. 원인 분석
확인: TrySupportBypass는 최대24후보를 차례로 검사한다. 각 후보는 진입 지지→목표 바닥→navmesh projection→경로/계속 경로→구간별0.08m 지지 검사 뒤 점수를 비교한다. 최종 점수는 FlatDistance(Position,sampled)+FlatDistance(sampled,PrimaryGoal), 갱신 조건은 cost<bestCost다.
후보 제안: projection 뒤 이 동일 점수를 계산해 이미 유효한 최선보다 같거나 나쁘면 FindPath 및 이후 세부 검사를 생략한다. 선택될 수 있는 후보의 안전 검사는 모두 유지한다. 후보 순서·동점 우선순위·실행 시 충돌/지지 검사·재시도 시계는 변경하지 않는다.
조건부 논리: 입력과 월드가 호출 동안 안정적이고 질의 생략에 의미 있는 부수효과가 없다면 cost>=bestCost 후보는 원본에서도 선택되지 않는다. 동점은 원래도 교체하지 않는다. NaN/비정상 입력에서는 최적화를 하지 않는 방어가 필요하다.
미확인: 실제412호출 중 지배되는 후보 수, 월드/네이티브 질의의 부수효과, 전체 CPU 이득. 원본 이후 거절 카운터는 생략으로 달라질 수 있어 별도 dominated 카운터로 구분해야 한다. 이것은 현재 입증된 안전 최적화가 아니다.
중복 확인: 기존 진입 검사 재사용은 첫 SupportStepClear의8방향 중복 제거였다. 평지 국소 -3.06%로5% gate 미달했다. 이번 제안은 경로·세부 지지 검사까지 가기 전에 최종 선택에서 지배되는 후보를 배제하는 다른 범위다. 읽은 관련 기록에서 동일 후보의 기존 시험은 확인하지 못했으며, 전체 이력에 없었다고 단정하지 않는다.

## 5. 검증 수준
기존 증거의 호출 소유자 대조 완료. 신규 후보는 미구현·미실행·성능 미입증이다. 기능·시각·성능 최종 수용 모두 아니다. 20ms p95 목표는 유지한다.

## 6. 변경 사항
분석 문서만 추가. 현행 소스 SHA256 EB43C45EF31E0E48B222CA3BF3CE03B4B2A7D4B6EF2CE580BA926C39019A2B82(DGBNativeGround.c)을 기준으로 읽었다. 게임 코드·PBO·서명·원시 기록·실행기 변경 없음. 새 서명 대상 없음.

## 7. 원시 증거
ANIM-contact-owner-direct1의 npc-function-times.txt, npc-selected-owners.txt, analysis.json, interpretation.json 및 기존 report.md. support-entry-reuse1/method-report.md. 현행 DGBNativeGround.c의 TrySupportBypass/DriveGround/Update, DGBGroundAgent.c의 MoveTo/Floor, DGBNativeTraversal.c의 Walk/SurfaceWalk.
공개 요약은 내부 경로·PID·바이너리·개인키를 노출하지 않는다.
[기존 8→64 호출 소유자](operations/npc-sixtyfour-named-owner-20261108.md) · [기존 진입 검사 후보 판정](operations/support-entry-method-disposition-20261108.md) · [최신 변환 분석](operations/transform-update-attribution-20261108.md).

## 8. 잔여 문제
다음 단일 산출물은 기존 메서드 하네스에서 원본/지배 후보 배제의 선택 경로·반환값·복구 상태 비교와 실제 생략된 고비용 호출 수를 함께 판정한 표다. 구현 전 위험과 검증 비용을 이 문서로 제시한다.
검증 순서: 격리 클래스(운영 코드 미수정), 기존 평지/벽/지붕/가장자리/낙하·복구 조기반환 조건의 동일 입력 비교→대표 평지5% 국소 gate 및 제외 조건3% 악화 제한→통과할 때만8개체 상태 계약과64 전체CPU 단일쌍 선별. 전체CPU5% gate를 못 넘으면 긴 회귀/반복으로 확장하지 않는다. 선별을 통과해야 동일 조건 독립 반복3쌍 및 시각/기능을 별도 검증한다.
위험: 질의 생략의 부수효과·동적 월드·진단 카운터 의미·실제 호출 수 부족. 국소 gate 통과는 전체 비용 성공이 아니다. 기대 절감률은 제시하지 않는다.
