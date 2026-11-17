# Unity 8개체 리뷰 — 로그·월드 재구성 비용 비교

작성: 2026-11-17T23:06:29+09:00  
측정 소스 식별자: `source-76162ff2024bce02` (소스 SHA256 집합 식별자이며 배포 실행파일 빌드/서명 ID가 아님)

## 1. 작업 목표

같은 `HordeReview.unity`의 8개체·계단·고정 플레이어·동일 카메라 설정에서 상세 기록 및 반복 형상 재구성 비용만 줄인다. 충돌·지지 성공 강제, 물리 검사 생략, dt/애니메이션/IK 반복 수/군집 정책 변경은 하지 않는다. DayZ와 UnityHordeLab은 수정하지 않는다.

## 2. 실제 수행

- 1차: baseline → bulky-log suppression → log+exact mesh geometry reuse, 역순을 포함한 총 9회(각 조건 3회).
- 2차: baseline/combined를 순서 교차하여 총 6회(각 조건 3회). combined에는 정확한 콜라이더 속성으로 만든 revision 문자열 재사용도 포함한다.
- 매 질의에서 정점·인덱스·변환을 현재 값과 정확하게 비교한다. 불변인 검증 형상/문자열만 재사용한다. 월드 전체 콜라이더 탐색, 경계·제외 규칙, 모르는 형상의 Unsupported 처리, 실제 Sphere/Path·지지 검사는 유지한다.
- 로그 축소는 큰 crowd/steering JSON과 perception·combat 반복 기록을 생략한다. 이동·힘·지각·전투 계산 및 필요한 관측 콜백은 남긴다. 상세 기록 기본값은 기존과 동일하며 효율 리뷰 메뉴에서만 선택한다.
- 형상 12개 + revision 19개 변경 감지 대조를 통과했다. 이동/회전/크기, 동일 메시 정점·인덱스 변경, 열린 메시, 다중 submesh, 메시 교체, 박스 크기·중심, enable/active/layer/trigger, 알 수 없는 새 콜라이더, 제거·영역 출입 등을 포함한다. Sphere 결과와 revision 문자열을 기존 경로와 직접 대조했다.
- 비용 측정 종료 후에만 실제 Game 뷰 framebuffer를 별도 캡처했다. 캡처는 CPU 포즈 값의 대체물이 아니다. PNG 모듈을 추가하지 않고 기본 Texture2D readback + 작은 RGB 파일을 썼다. 캡처 중 성능은 벤치마크에서 제외한다.

## 3. 측정 결과

| 항목 | 원본 3회 | 후보 3회 |
|---|---:|---:|
| LateUpdate interval p50 ms | 1597.16–1621.83 | 330.05–367.03 |
| LateUpdate interval p95 ms | 2799.53–2886.89 | 440.21–541.37 |
| LateUpdate interval p99 ms | 2799.53–2886.89 | 440.21–541.37 |
| Gen0 collections / window | 782.00–800.00 | 59.00–71.00 |
| Geometry read requests / window | 129543.00–129543.00 | 128544.00–162906.00 |
| Coordinator Step elapsed ms / simulation tick | 377.55–384.33 | 78.64–89.38 |

틱당 coordinator 경과시간 중앙값 감소: **78.2%**. Unity 프로세스 CPU 시간의 틱당 범위는 원본 380.4–387.6ms, 후보 80.1–92.0ms였다. 프로세스 전체 CPU와 coordinator inclusive 경과시간을 함수 self-time으로 해석하지 않는다.

원본 측정 구간은 9→81틱(72틱), 후보는 8→83/10→81/10→82틱(75/71/72틱)이다. 동일 scene 초기 조건이나 프레임당 catch-up 묶음 때문에 관측 경계와 실제 질의량은 완전히 같지 않다. 따라서 틱당 비용도 병기한다. 후보 질의량은 더 적다고 볼 수 없으며 원본과 정확히 동일한 궤적을 입증한 시험도 아니다. 각 구간 LateUpdate 표본은 18–19개 정도여서 p95/p99는 높은 불확실성이 있고, p99가 p95와 같다고 tail 문제가 없다는 뜻이 아니다.

1차에서 로그 축소만으로는 명확한 처리시간 개선이 없었다(약 26.1–27.3초). 메시 형상 재사용을 더한 조건은 약 20.7–22.4초, 기존 원본은 26.7–27.2초였다. 2차의 큰 개선은 형상 재사용만으로 얻은 수치로 보고하지 않는다. 추가 revision 문자열 재사용 및 확대된 로그 축소를 포함하는 묶음 효과다.

실제 화면 캡처: 원본 5개, 후보 17개의 서로 다른 framebuffer를 얻었다. 원본 캡처 사이 간격 0.749–2.033초, 후보 0.261–0.456초. 후보는 최대 4Hz 캡처 제한과 readback 비용이 있으므로 이 수치를 표시 FPS로 환산하지 않는다. 원본의 긴 정지는 줄었으나 후보에도 뚜렷한 계단식 움직임이 남는다. **시각 품질 최종 수용은 미완료**다.

## 4. 원인 분석

확인: 큰 JSON 행 생성을 줄이는 것만으로는 주된 지연을 해소하지 못했다. 동일 월드의 메시 해석·변환/형상 문자열 재구성을 반복하는 경로를 재사용했을 때 세 번 모두 처리 비용이 크게 줄었다. 측정 구간의 메시 재구성은 원본 129,543회에서 후보 0회로 줄었고 요청은 128,544–162,906회 남았다. 0회는 warm-up 이후 이미 검증된 형상의 재구성이 없었다는 뜻이며 실제 충돌 검사가 0회라는 뜻이 아니다.

추정: 남은 전체 콜라이더 발견/부모 검사, revision 유효성 검사, 실제 support/path 검사, 반복 IK 등이 잔여 비용 후보다. 이번 측정으로 각 self-time을 분리하지 않았다.

미확인: OS Present/GPU 시간, 실제 하드웨어 표시 간격, 할당 바이트·할당기 self-time, GC 소요시간, 64/1000개체 비용, 장시간 동적 맵·문·시체 변경 상황. Gen0 횟수 감소를 할당량 또는 GC 시간 감소율로 쓰지 않는다. 다른 PC 작업/전면 창 변동을 완전히 차단한 전용 환경은 아니었다.

## 5. 검증 수준

- 진단 성공: 선택한 재구성 경로에 반복 가능한 개선 근거 확보.
- 국소 회귀 통과: 31개 정확성/변경 감지 대조. 저장 리뷰 씬 SHA256 불변.
- 반복 검증 완료: 같은 8개체 씬에서 원본/최종 후보 각 3회 비용 비교.
- 실제 표시 관측 완료: framebuffer 캡처와 타임스탬프 확보. 부드러움 최종 수용은 실패/보류.
- 전체 기능 회귀·전리품·피격·사망·8/64 전원 terminal 성공 판정은 이번 범위에서 하지 않았다. 설정을 유지했다는 사실을 이들 시험 통과로 바꾸지 않는다.

## 6. 변경 사항

- `Assets/Playground/Runtime/DGCourseMeshGeometry.cs`: opt-in 정확 콘텐츠 재사용·초기화·요청/재구성 카운터.
- `Assets/Playground/Runtime/DGTraversalFixtureWorld.cs`: 정확 현재 속성에서 revision 문자열 부분/최종 문자열 재사용. 매번 콜라이더 발견과 변경 검사는 유지.
- `Assets/Playground/Runtime/DGTraversalCrowd.cs`: compactReviewDiagnostics 선택 및 동일 조건 Step 경과시간 계측.
- `Assets/Playground/Runtime/DGTraversalPerception.cs`, `DGCombatConnection.cs`: 큰 반복 로그만 선택적으로 생략.
- `Assets/Playground/Runtime/DGReviewPerfProbe.cs`, `Assets/Playground/Editor/DGReviewPerfChecks.cs`: 재사용 가능한 국소 비교·변경 감지 검사·시각 캡처·효율 리뷰 메뉴.
- 저장된 씬·모델·클립·Packages/manifest는 수정하지 않았다. PBO 수정/서명 작업은 해당 없음.
- 메뉴: **Horde Research → Open efficient 8-actor review**. 원래 리뷰와 상세 진단 기본값은 보존한다. 효율 리뷰 프로필은 unsaved observer이며 F5 scene 재시작에 다시 연결하도록 했다.

## 7. 원시 증거와 재현

Unity 6000.3.23f1, Built-in, DX12 Editor, 8개체, 원래 계단 씬, 고정 표적, 초깃값/카메라/진단 시각화 동일 설정. Game와 Scene 뷰가 모두 열린 기존 레이아웃을 사용했다. 물리/AI dt=.05, 최대 catch-up 4회는 변경하지 않았다.

- `Artifacts/ReviewPerf/20261117-diagnostics-geometry/`: 1차 run-0…8, CSV, 변경 전 두 소스와 씬 사본, SHA256.
- `Artifacts/ReviewPerf/20261117-revision-reuse/`: 2차 run-0…5, CSV, controls, before-source, measured-source, measured-build-sha256.json.
- `Artifacts/ReviewPerf/20261117-visual-review/`: baseline/combined.json, 실제 framebuffer, 실제 간격을 반영한 baseline.gif/combined.gif, contact-sheet.png.
- `Artifacts/ReviewPerf/20261117-revision-reuse/evidence-manifest.json`: 최종 종료된 증거·소스의 해시. 기존 로그는 덮어쓰지 않았고 JSON/JSONL 구문·경로·해시를 검증했다. 현재 실행 중인 효율 리뷰의 가변 Editor 로그는 고정 증거 해시에 넣지 않았다.
- 원시 SharedTraversal 경로는 각 run JSON의 tracePath 및 manifest에 연결한다. 닫힌 원시 기록 총량 약 94.8MiB, 별도 화면 자료 약 9.4MiB. 원시 자료와 자산은 공개 저장소에 자동 업로드하지 않는다.
- 처음 계측 도우미 필드 이름 오류 및 선택 모듈 부재에 따른 컴파일 실패가 있었고 수정했다. 실패 Editor 로그는 보존했다. 비용 결과는 성공적으로 컴파일된 실행에서만 얻었다.

## 8. 잔여 문제·다음 실험

후보도 실시간 1배 진행과 충분한 프레임 부드러움을 달성하지 못했다. 다음 단일 산출물은 **남은 coordinator 비용을 월드 발견/유효성 검사와 실제 support/path·포즈 계산으로 분리하는 작은 범위표**다. 현재 8개체 하네스를 재사용하고, 결과가 다음 수정 지점을 바꿀 정도로만 계측한다. 객체 수 확대나 충돌 삭제, 새 호드 정책 개발로 전환하지 않는다.

compact 리뷰에서는 전체 raw contact 감사에 필요한 큰 행이 생략된다. 상세 감사가 필요하면 기존 verbose 프로필을 사용한다. 이 변경으로 DayZ 실행 경로 또는 수백/수천 개체 성능이 개선됐다고 주장하지 않는다.
