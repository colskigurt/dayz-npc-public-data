# DayZ NPC AI 재개 — B8FB 실제 8→64명 검증

B8FB 실제 8→64명 재접근 검증: 최종 도착과 고지대 잔여 2개체

작성: 2026-10-28T05:44:35.889391+09:00

1. 작업 목표

B8FBAB74의 틱 간 입구 거부 전달과 실제 재접근 보행을 8→64명 격리 환경에서 확인한다. 정상 밀림·낙하·복귀를 보존하고 중간 고지대 추적과 최종 도착을 별도 집계한다.

2. 실제 수행

- 후보 보관본 24개 해시 대조 후 기존 서명 PBO 3개를 두 실행에서 검증했다. 새 게임 코드/PBO 변경·재빌드는 없다.
- activeReference, 8명/180초와 64명/300초. StateContract·DriveAttribution·extended egress·first/near-first step·endpoint lookahead·terrain contact owner·pursuit path revalidation·admission reapproach 사용. VSync on, 동일 그래픽 seed, 자동 정규 종료.
- 기존 분석기 5개로 상태 전이·출구 lease·재접근·HIGH_HOLD 표본·갱신 간격을 분석했다. 화면 캡처는 8명 장면 존재 확인에 한정되며 시간적 품질 영상은 없다.

3. 측정 결과

|항목|8명 053114|64명 053554|
|---|---:|---:|
|최종 FOLLOW/ARRIVED|8/8|64/64|
|HIGH_HOLD 내 미도달|0|2 (6·32번)|
|낙하 후 FOLLOW 복귀|10|76|
|재접근 ARRIVED / 목표변경 취소|1 / 0|15 / 1|
|재접근 계약 위반|0|0|
|출구 복구 DONE / 계약 위반|2 / 0|10 / 0|
|등반 종료 검증 실패|0|0|
|foreground 미션 dt p50/p95/p99|20/33/35ms|30/72/95ms|

각 조건 반복 1회. 64명 HIGH_HOLD 표본 구간 45.083~104.927초. 물리 전이 916개가 같은 호출 계측과 연결됐다. 미션 dt는 Present/GPU 지연이 아니며 상세 계측 비용을 포함한다. 갱신 콜백 약 13.96/15.27Hz는 화면 애니메이션 프레임률이 아니다. 20ms p95 미달, 최적화 개선 판정 불가.

4. 원인 분석

사실: 새 분기가 두 실행에서 실제로 동작했다. 64명에서 재접근 15회는 4.5cm 허용 간격 안에 도착했고 1회는 INTENT_CHANGED로 취소됐다. 미종료 intent와 출구 lease 위반은 없다. 첫 3ECC 후보의 분기 0회와 달리 B8FB 전달 수정의 실행성은 확인됐다.

사실: 6번은 50.805초 낮은 면에서 PRESSURE_SUPPORT_LOSS로 낙하, 75.382초 LAND, 82.801초 FOLLOW로 복귀했다. 64명 HIGH_HOLD 표본 113개 중 LANDING_SPACE_BLOCKED 45개, NATIVE_JOINT_WORLD_2 25개다. 이는 표본 수이며 지속 시간이나 충돌 원인을 독립 확정하는 수치가 아니다.

사실: 32번은 51.257·78.644·95.218초 낮은 면에서 같은 지지 상실 낙하를 요청했다. 72.874→75.055초와 87.873→89.922초 지면→낮은 면 등반은 완료됐고 이어 다시 낙하했다. 종료 지지 질의 8개는 CLEAR/true다. 정지한 재접근 intent나 CLIMB_EXIT_VALIDATION과 구분된다.

추정: 착지 및 본 포즈의 정적 컨테이너 접촉과 입구 정렬 중 군집 압력이 고지대 복귀를 늦췄을 수 있다. 실패 시점 LandingSpace의 전체 게이트와 실제 접촉 개체를 모두 기록하지 않으므로 근본 원인으로 확정하지 않는다.

반증/한계: 최종 64/64가 중간 고지대 성공을 뜻한다는 해석은 반증된다. 3ECC의 미도달 11명→이번 2명은 실행 간 기술적 관찰이며 통제 반복 A/B의 인과 효과나 안정성 수용이 아니다. 정상 낙하를 실패로 재분류하거나 지지 검사를 완화하지 않았다.

5. 검증 수준

- 진단 성공: B8FB 실제 재접근 실행·완료, 두 잔여 개체의 상태 흐름 분리.
- 이번 8명 기능 묶음: 해당 한 실행에서 중간/최종 도착 및 계측 계약 통과. 반복 검증 완료 아님.
- 64명: 최종 도착·출구 계약은 해당 실행 통과, 고지대 중간 기준 미달. 전체 회귀 통과·최종 수용 아님.
- 과거 74개 회귀 검사는 보존했으며 이번에 재실행하지 않았다.
- 시각 부드러움·CPU/표시 지연 원인·성능 최적화: 미수용. 네이티브 애니메이션 평가/그래프·GPU·할당/GC 직접 귀속은 이 실험에서 미측정.

6. 변경 사항

게임 코드와 PBO는 변경하지 않았다. Integration build B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185, mission source F8356C4F1E0286DCE7ED83F0137E68061249D48ABEB175942D44A8A048D704D3. 후보 보관본 해시 일치 및 기존 서명 3개 검증 성공. 새 서명 수행은 없다.

새 파일: runtime 두 개의 분석 파생본, checkpoint_horde_admission_runtime_20261028.py, 이번 체크포인트·복사본·선정 근거 패킷. 기존 진행 JSON에 현재 결과를 추가한다. 과거 보고서와 원시 자료는 덮어쓰지 않는다. B안 공개 저장소만 갱신한다.

7. 원시 증거

실행 ID runtime-20261028-053114 및 runtime-20261028-053554. 각 profile의 launch.json, native-ground.tsv, native-natural-player.tsv, native-state-contract.tsv, native-admission-reapproach.tsv, native-pose-cadence.tsv와 state-contract-report.json, missed-surface-analysis.json, admission-reapproach-analysis.json, egress-clearance-analysis.json, controlled-analysis.json. 원문 경로와 SHA-256은 아래 로컬 무결성 목록에 기록한다. 공개본은 선정 필드·함수 발췌만 제공한다. 새 영상은 수집하지 않았다.

8. 잔여 문제

1. 6·32번에 대해 낙하→착지 차단→복귀/재등반 구간을 기록 입력으로 재생하고 LandingSpace 게이트/충돌 대상과 정렬 중 압력 소유권을 확인한다. 정상 낙하를 보존하며 도착 위치 순간이동이나 검사 완화는 하지 않는다.
2. 동일 B8FB·그래픽·부하에서 재접근 flag OFF/ON 대조와 반복 실행으로 실행 간 변동을 분리한다. 단일 유리한 실행으로 성공 선언하지 않는다.
3. 애니메이션 별도 실험: 1개체/64개체 AI·이동 OFF, 64개체 정적 포즈 active, 64개체 완전 active. 실제 화면 프레임과 CPU 포즈를 시간으로 맞추고 버퍼 연속성을 조사한다. 추측 보간은 보류한다.
4. 성능 별도 실험: AI/경로/복구/접촉 및 노출 가능한 갱신 비용을 동일 조건에서 귀속한다. 네이티브 평가·렌더·GC의 계측 불가 부분은 명시한다.
5. 사용자 요청한 골 재개는 실행 중이며 이 보고서는 안전 종료한 기능 묶음의 체크포인트다. 전체 목표는 미완료다. 운영 서버는 수정하지 않았다.

원본 보고서 ID: 2026-10-28_054435_889391+0900_npc-admission-latch-runtime.md
원본 SHA-256: f320a6700aaca369f0aa67dd658620d3dff39c577106d600cd3779b668338002
이 텍스트는 복사용 파생본입니다. 수치·검증 한계는 원본과 같으며 상세 파일 해시 목록은 원본에 보존되어 있습니다.

선정 함수·대표 기록: [검토 패킷](../downloads/checkpoints/2026-10-28_054435_889391+0900_npc-admission-latch-runtime_evidence.txt)

[이전 기능 분석](../downloads/admission-report.txt) · [이전 애니메이션 분석](../downloads/animation-report.txt)
