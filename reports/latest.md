# Lazy crowd eligibility — 국소 선별 결과

## 1. 작업 목표
기존 eager eligibility의 희박 조건 악화를 반복하지 않고, Crowd.Update 안에서 처음 필요한 개체의 이동 가능 판정만 재사용하는 국소 후보를 선별한다. 물리 질의·힘 계산 순서·지속 등반·낙하·애니메이션 갱신은 유지한다.

## 2. 실제 수행
현재 13개 PBO를 변경 없이 사용했다. 실제 NPC 0개, 합성 Body 64개씩 기준/후보를 구성한 기존 하네스에서 512 상태 입력과 갱신 밖 호출·세대 넘침·재진입 가드·개체 수 감소/증가·OpenTraversal=false fallback을 비교했다. 밀집/희박/중간 배치별 1000 Update씩 3쌍, 두 번째 쌍 역순 실행. 캐시 초기 resize 비용은 측정 안에 포함. 원본 pair 함수는 캐시 부분 제거 시 복원되는지 확인했고 Update는 super를 사용한다.
초기 미션은 74행 문법 오류로 실패했다. 실패본을 보존하고 revision2에서 local/group 식별자를 바꿔 컴파일했다. 첫 상태 조회의 프로세스 부재 판단은 부정확했으며, 이후 정확한 PID·시작 시각·profile을 대조해 컴파일 실패 서버를 종료한 기록이 우선한다.

## 3. 측정 결과
| 조건 | 쌍 후보/Update | 판정 계산 개체/Update | ON/OFF 변화 3블록 | 중앙값 |
|---|---:|---:|---|---:|
| 밀집 |2016|64|-10.07%, -13.71%, -14.03%|-13.71%|
| 희박 |0|0|+0.10%, +0.70%, -0.48%|+0.10%|
| 중간 |480|64|-25.97%, -11.91%, -9.28%|-11.91%|
524개 trace 행(512 상태 입력 + 경계/타이밍 준비 행), 누적 불일치 0. 사전 동결 국소 gate 통과. 독립 프로세스 1회이며 3개 독립 실행이 아니다. TickCount 원시 단위는 미확인이라 비율만 사용한다. 중간 첫 블록의 큰 차이는 편차로 보존하고 대표값으로 선택하지 않는다. 전체 CPU·Present p50/p95/p99·실제 애니메이션 품질은 미측정. 실제 8/64개체 기능 시험 미실행.

## 4. 원인 분석
확인: 기존 eager 방식은 이웃이 없어도 모든 개체를 미리 판정했다. 이번 방식은 해당 Update에서 쌍 필터를 통과해 필요한 개체만 판정하며 희박 조건 계산은 0이었다. 힘/속도/pressure/entry 결과는 시험 범위에서 동일했다.
추정: 반복되는 스크립트 상태 조회를 줄이는 것이 관찰된 국소 시간 감소에 기여한다. 할당·캐시·실행 순서의 독립 self-time은 분리하지 않았다.
미확인: 실제 8/64개체의 전체 CPU 절감, 라이브 상태 변경 및 native callback 재진입에서의 동작. 가드 시험은 실제 재진입 callback을 주입한 것이 아니다. 물리 질의 자체가 줄었다는 주장은 하지 않는다.

## 5. 검증 수준
진단 성공/국소 선별 통과. 실제 기능 회귀·화면 품질·전체 성능·반복 실행·최종 수용은 미완료. 정상 낙하를 실패로 간주하거나 모든 개체의 컨테이너 동시 점유를 성공 기준으로 삼지 않는다. 20ms p95는 목표다.

## 6. 변경 사항
수정은 revision2의 별도 미션 init.c에 한정한다. 기존 게임 소스/PBO/자산/운영 서버는 변경하지 않았으며 신규 PBO가 없어 서명·검증 대상도 새로 생기지 않았다. 빌드는 기존 13개 모드 조합, 후보는 LAZY-ELIGIBILITY-method1-r2. 원본/입력/미션 해시는 source-proof 및 plan에 연결한다.

## 7. 원시 증거
동일 revision2의 method-contract.json, source-proof.json, plan.json, launch.json, terminal.json, analysis.json, mission/init.c, server-profile/crowd-lazy-eligibility.tsv 및 crowd-lazy-eligibility-cost.tsv와 script 로그. 첫 실패의 error/crash 로그와 terminal-reconciled.json도 보존한다. 자동 생성된 첫 terminal-observed.json의 processAbsent는 정정 기록과 함께 읽어야 한다. 서버는 RequestExit(0) 기록 후 프로세스 종료 확인, OS exitCode -1/forced=false이며 native crash 로그는 이번 revision2에 없다.

## 8. 잔여 문제
다음 단일 산출물은 opt-in 8개체 shadow/ON 동작 비교와 전체 CPU 기여 판정이다. 국소 결과를 전체 CPU 절감으로 환산하지 않는다. 그 비교가 유효할 때만 64개체 비교로 확대한다. 바닐라의 내부 Ray/Sweep 횟수·접촉 캐시 재사용은 아직 미확인이고 스크립트 경계 횟수로 대신하지 않는다. 기존 64개체 프로파일은 Ray/Sphere inclusive 표본 17.84%, 결손 stack 26.13%이며 이 수치들을 합산하거나 현재 후보의 개선율로 연결하지 않는다.
