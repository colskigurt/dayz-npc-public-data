# 단일 파트 수동 8개체 시험 결과

## 1. 작업 목표
사용자가 직접 조작한 단일 파트 후보 8개체의 추적·등반·하강·피격·사망 기록을 확인한다. 정상 낙하 복귀와 미해결 문제를 구분하며 새 구현·재시험은 하지 않는다. 골은 일시정지 상태로 유지한다.
## 2. 실제 수행
종료된 시험의 gameplay, combat, 상태 계약, 접촉 실패, 피격 표면, 받은 피해, 사망·전리품, 서버 종료 로그를 교차 분석했다. 8개체 ID 범위와 표 형식을 검증하고 원시 파일 해시를 기록했다. 종료 코드는 retained-process 기록에서 확인했다. 코드·PBO는 수정하지 않았다.
## 3. 측정 결과
실행 약94.51게임초, 논리8·렌더8·수신체8, 독립1회.
- 추적: 8/8 인식·이동, 개체별 누적76.01~101.24m.
- 등반: 전원 등반 카운터 증가, 합계18회. 고지대 area 관측8/8이나 이것만으로 전원 고지대 등반 완료를 증명하지 않는다.
- 낙하: FALL 전환4건 모두 LAND 후 FOLLOW 복귀. 3건은 약1.05~1.07초, ID5는10.742초.
- 감염자 공격: 시작25회, 적중11회.
- 받은 총기 피해: 원시 콜백17건, 전부 MakarovIJ70/Bullet_380. 부위 Torso8/RightArm4/Head2/LeftLeg1/LeftArm2. 치명타·중복 콜백이 포함돼 발사수나 독립 명중수로 해석하지 않는다.
- 피격 반응 카운터6회(4개체), 지연 피격 반응 만료1건(ID5,FALL,42.909초).
- 사망: 8/8 DEAD 및 DEATH_POSE_COMPLETE 기록.
- 시체: 7/8 접지·인벤토리 준비 및 lootSeeded. ID7은 WAIT_FOR_SETTLED_CORPSE, 사망 포즈 완료 후49.6288초가 지나도 미완료.
- 종료:23:10:41 KST 종료 요청 후23:10:47 KST RetirementObserved NULL 예외2건. 서버 exit=-1, 강제 종료 아님, 정상 종료/전체 기능 통과로 판정하지 않는다.
이번 시험의 CPU/Present/GPU p50·p95·p99는 미측정이며 프레임·시각 품질 수용도 하지 않았다.
## 4. 원인 분석
확인된 결함/우선 검토:
1) ID5 등반의 LIFT_LIMIT 진단7건에 실제 DGNativeCorpseContact가 검출됐다. 이 개체는 총 접촉 거부238회, 경로 실패25회, 낙하 복귀10.742초와 SOURCE_HEIGHT_FIT_LIMIT 이력이 있다. 시체 충돌체가 해당 구간을 방해한 근거이며 모든 거부·지연의 단일 원인이라고 단정하지 않는다.
2) ID7 사망 포즈는 완료됐지만 접지 완료가 안 되어 전리품 준비가 계속 대기했다. 짧은 종료 시점 오차로 보기 어려운 약49.6초의 미완료가 관측됐다.
3) 등반/낙하 도중 지연된 피격 반응1건이 만료됐다. 피해 판정과 실제 피격 모션 출력은 별개다.
4) 수동 미션의 Closing 분기에서 개체 삭제 후 Parts/Bodies 참조를 남기고 RequestExit 후의 다음 OnUpdate가 이를 다시 읽는다. 실제 스택은 OnUpdate152행 RetirementObserved. 같은 종료 코드가 기존5파트 수동 미션에도 있음을 확인했으나 기준 실행의 동일 예외 재현은 미확인이다. 단일 모델 전용 엔진 결함으로 단정하지 않는다.
미확인: 근접 도끼 피해는 이번 받은 피해 로그에 없다. 피격 표면은 Hit_MeatBones9/UNRESOLVED6 이벤트로, 실제 피·불꽃·음향을 로그만으로 판정할 수 없다. 영상이 없어 애니메이션의 실제 부드러움도 판정하지 않는다.
## 5. 검증 수준
수동 시험의 부분 기능 확인·결함 진단 성공. 전체 기능 회귀 통과/반복 검증/최종 수용 아님. 피해·사망 상태 기록과 실제 화면 품질을 분리한다. 64개체 검증을 대신하지 않는다.
## 6. 변경 사항
분석 스크립트와 JSON/보고서만 생성했다. 게임 코드·PBO·운영 빌드 변경 없음. 사용한 단일 모델/격리 어댑터는 기존 서명 검증된 해시 고정 입력이다. 새 모드 빌드·서명·재실행 없음. 첫 파생 요약의 이벤트 phase 열 위치를 고친 manual-findings2.json을 사용하며 이전 파생본과 원시 기록은 보존했다.
## 7. 원시 증거
실행 ID MANUAL-single224-review1-20261109-launch2.
manual-single224-review1/manual-analysis1.json,manual-findings2.json,analyze-manual1.py,summarize-findings2.py.
case2/runtime-once/server-profile의 gameplay-eight.tsv,combat-eight.tsv,native-state-contract.tsv,native-contact-failures.tsv,native-combat-events.tsv,native-received-damage.tsv,native-hit-surfaces.tsv,script 및 crash 로그.
서버 종료 기록 server-20261109T1410499425314Z.json.
## 8. 잔여 문제
우선순위는 종료 재진입 방지, ID7 시체 접지/전리품 대기, 시체 충돌체와 ID5 등반 거부 관계, 지연 피격 반응 정책이다. 실제 수정은 이번 요청 범위 밖이므로 하지 않았다. 기존 기준 빌드 재현 여부와 후보 전용 차이를 먼저 구분해야 한다. 이 수동 시험을 이유로 주력 CPU 최적화를 기능 개발 전체로 전환하지 않는다. 기존 서버CPU 감소2쌍 결과는 별도 근거이며 이번 사망이 많은 수동 시험의 비용으로 대체하지 않는다. 골은 계속 일시정지다.
