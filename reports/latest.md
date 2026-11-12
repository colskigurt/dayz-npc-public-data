# 네이티브 이동 벽·발판 판정 — 현재 모델의 벽 검사 직접 대체 기각

## 1. 작업 목표
단순 평지 이동 성공을 물리 안전성이나 최적화 성공으로 오인하지 않도록, 현재 모델의 Native Move 및 기존 DGNativeCommand unmanaged script-root 경로를 벽·발판 조건에서 확인한다. 기존 NPC의 지지·벽·접촉 검사는 제거하지 않는다.

## 2. 실제 수행
같은 격리 씬의 분리된 네 레인에서 Native Move 벽, script-root 벽, Native Move 발판, script-root 발판을 20게임초 실행했다. 현재 single224 모델의 단독 호스트이며 GroundBody·피해 수신체·추적 AI·전체 NPC 솔버는 연결하지 않았다. StaticObj_Container_1C의 벽과 윗면은 진단 Ray로 실제 검출했고, 개체 생성 및 물리 body 유무를 확인했다. 진단 Ray 횟수는 네이티브 내부 검사 횟수가 아니다.
첫 미션은 opaque FileHandle에 숫자 0을 대입해 컴파일 실패했다. 실패 로그와 종료 영수증을 보존하고 revision2에서 별도 bool 폐쇄 상태로 수정했다. 성공 실행과 실패 실행을 합치지 않았다. 새 PBO나 게임 자산 변경 없이 기존 서명 검증된 프로브를 재사용했다.

## 3. 측정 결과
실행 NATIVE-ROOT-world-gate1-r2, 독립 실행 1회, 성능 비교쌍 0회. 개체별 182표본, 총 728행. 네 fixture Ray 검출 성공, 4개체 생존, 스크립트 오류 0, 지하 긴급 중단 없음.

| 조건 | 관찰 결과 | 판정 |
|---|---|---|
| Native Move 벽 | 실제 Ray 벽면 Z=10221.128906 뒤로 3.896초에 진입, 7.349초에 상자 뒤까지 통과 | 현재 설정에서 벽 검사 직접 대체 기각 |
| script-root 벽 | 같은 벽면 뒤로 3.896초에 진입, 7.349초에 상자 뒤까지 통과 | 현재 설정에서 벽 검사 직접 대체 기각 |
| Native Move 발판 | 정지 중 root가 Ray 윗면보다 0.003845m 높게 유지, 후방 이탈 4.536초, 지면 근접 5.023초 | 초기 윗면 지지와 이탈 후 하강 관찰 |
| script-root 발판 | 동일한 초기 높이 오차와 이탈·지면 근접 시각 관찰 | 초기 윗면 지지와 이탈 후 하강 관찰 |

벽 조건은 X가 상자 범위 안이고 root Y가 지붕보다 낮은 상태로 뒤까지 통과해 옆 우회·위로 넘기와 구분했다. 실제 Ray 평면을 기준으로 했으며 bounding box를 곧바로 단단한 형상으로 가정하지 않았다. Native PreAnim 최대 405회, script PreAnim 최대 404회, script root 적용량 36.958m. CPU/Present p50·p95·p99, 네이티브 query 수, 화면 부드러움은 미측정이다. 8/64개체 전체 기능 시험은 이번 실행에 해당하지 않는다.

## 4. 원인 분석
확인: 현재 모델·입력 override·명령 설정에서 두 이동 경로는 검출된 벽 통과를 막지 않았다. 따라서 기존 벽 검사를 삭제하고 그대로 교체할 근거가 없다. 이전의 실제 root 이동 성공은 유지되지만 물리 대체 가능성으로 확대하지 않는다.
확인: 윗면 지지는 유지됐고 가장자리 이탈 후 하강했다. 이 결과를 기존 애니메이션 발 지지 솔버와 동등하거나 전체 낙하 애니메이션이 정상이라는 주장으로 확대하지 않는다.
모름: 바닐라 감염자의 실제 네이티브 내부 지지·벽·접촉 검사 소유자, 호출 수, 캐시와 순수 비용. 이번 커스텀 모델 결과가 모든 DayZ Native Move에서 벽 충돌이 없다는 뜻은 아니다. 유사 궤적만으로 두 경로의 내부 구현이 같다고 판정하지 않는다.
분류 수치 0.3m 등의 상세 서술 기준은 실행 후 위치 trace 검토 전에 작성됐다. 사전 등록된 수용 계약이 아니다. 원래 실행 전 범위·중단 계약과 사후 서술 계획을 분리 보존했다.

## 5. 검증 수준
벽·윗면 검출 및 현재 설정의 벽 통과 확인: 진단 성공. 직접 물리 대체 후보: 미수용. 전체 NPC 회귀·시각 품질·반복 성능·최종 수용: 미실행. 성능 개선률 없음. 20ms p95는 계속 목표다.

## 6. 변경 사항
기존 rate100-isolation 연구 폴더 안의 격리 mission·분석·기록만 추가했다. 원본 게임 코드·PBO·모델·운영 서버·이전 실험은 보존했다. 재사용 프로브 PBO SHA256 5d7184ef91b44ff4a4a5ffb0ccca918cf20b46a75324d24af78cabc49f651db8. 기존 서명·검증 로그와 바이트 해시를 대조하며 새 서명 수행으로 기록하지 않는다. 성공 실행 서버 terminal exit -1/forced=false, 클라이언트 closed=true/forced=false이며 현재 프로세스 종료를 확인했다.

## 7. 원시 증거
동일 실행의 analysis.json, contract.json, analysis-plan.json, analysis-plan-clarification.json, mission init.c, world-gate.tsv 728행, world-gate-fixture.tsv 4행, server-terminal.json, client-closed.json 및 이전 실패 failed-terminal.json을 연결한다. 공개 파생본은 집계·선정 행·원본 해시만 담으며 로컬 경로·프로세스 정보·설치 자산·키를 제외한다. 원시 자료와 기존 보고서는 덮어쓰지 않는다.

## 8. 잔여 문제
벽 검사 직접 대체는 중단하고 기존 비용 연구로 복귀한다. Native Move 단독 평지 이동이나 같은 벽 시험을 반복하지 않는다. 이번 실패를 등반 기능 개발이나 전체 엔진 분석으로 확대하지 않는다.
다음 단일 산출물은 기존 바닐라/커스텀 비용·호출 주기·확인 수준 비교표와 변경 가능한 국소 후보 한 개의 선정 근거다. 전체 native 내부 Ray 횟수를 모른다는 이유로 모든 최적화를 보류하지 않는다. 기존 SupportSweepFast·Scratch·호스트 축소·caller attribution 결과를 재사용하며, 정상 밀침·낙하·착지·FOLLOW·재등반은 보존한다.
추가 소스 검토에서는 GroundAgent.MoveTo의 전체 peer 루프가 ManagedPeerContacts에서 건너뛰어지고 ConfigurePeerContacts가 OpenTraversal/CrowdDynamics/Crowd 존재 조건으로 이를 설정함을 확인했다. 따라서 이 루프를 현재 O(N²) 병목으로 단정하거나 기존 Crowd 공간 격자를 중복 구현하지 않는다. 실제 활성 실행 설정 확인 전 미선정이다.
